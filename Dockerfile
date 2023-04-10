FROM ubuntu:22.04 as builder

RUN apt update -y --fix-missing && apt install gcc -y

WORKDIR /app

# copy code/src files
COPY main.c ./

# we copy md4c
COPY ./md4c/ ./md4c/

# build the ./web app
RUN gcc -Wall -lpthread -s \
	main.c \
	./md4c/entity.c ./md4c/entity.h \
	./md4c/md4c-html.c ./md4c/md4c-html.h \
	./md4c/md4c.c ./md4c/md4c.h -o sdk

# copy content
COPY content/ content

# just for the public folder
# COPY public/ public

# We build the html pages and put that in /static
RUN /app/sdk build

# FIXME: delete this instruction later, since the builder should create html files
# FIXME: + minify the html
RUN mkdir /app/public && \
    echo "H3llo there!" > /app/public/index.html && \
    echo "Hexllo Page 2" > /app/public/page.html && \
    echo "Hello 404 !" > /app/public/404.html

# ----------------------- #
# ----- serve stage ----- #
# ----------------------- #
FROM nginx:stable-alpine3.17-slim as prod

# we copy the nginx conf
COPY ./nginx.conf /etc/nginx/nginx.conf

# we just copy the built web pages
COPY --from=builder /app/public/index.html /usr/share/nginx/html/
COPY --from=builder /app/public/page.html /usr/share/nginx/html/
COPY --from=builder /app/public/404.html /usr/share/nginx/html/

# we expose the port 80
EXPOSE 80
EXPOSE 8080

# and we serve
ENTRYPOINT ["nginx", "-g", "daemon off;"]
