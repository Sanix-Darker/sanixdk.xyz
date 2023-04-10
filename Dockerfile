# ----- compile stage ----- #
FROM ubuntu:22.04 as builder

RUN apt update -y --fix-missing && apt install gcc make -y

WORKDIR /app

# copy code/src files
COPY . .

# build the ./web app
RUN make compile

# We build the html pages and put that in /static
RUN mkdir ./public && make build

# ----- serve stage ----- #
FROM nginx:stable-alpine3.17-slim as prod

# we copy the nginx conf
COPY ./nginx.conf /etc/nginx/nginx.conf

# we just copy the built web pages
COPY --from=builder /app/public/* /usr/share/nginx/html/

# we expose the port 80
EXPOSE 80

# and we serve
ENTRYPOINT ["nginx", "-g", "daemon off;"]
