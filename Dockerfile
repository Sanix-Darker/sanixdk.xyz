FROM ubuntu:20.04 as builder

WORKDIR /app

RUN apt update -y --fix-missing && \
    apt install gcc -y

# Copy code/src files
COPY . .

# Build the ./web app
RUN gcc -Wold-style-declaration -Wall \
		-s main.c ./lib/*.c -o builder

# This step is to add headers/footers on  all files
# this step is to polish everything related to css or any resources involve
# We build the html pages and put that in /static
RUN /app/builder build

# at this step we should already have everything we need to be serve
FROM nginx:stable-alpine3.17-slim as prod-serve

# We copy the nginx conf
COPY ./nginx.conf /etc/nginx/nginx.conf
# we just copy the built web pages
COPY --from=builder /app/public/* /usr/share/nginx/html/
COPY --from=builder /app/public/blogs/* /usr/share/nginx/html/blogs/
# Fix related to css style
COPY --from=builder /app/public/assets/style.css  /usr/share/nginx/html/assets/style.css

# we expose the port 80
EXPOSE 80

# and we serve
ENTRYPOINT ["nginx", "-g", "daemon off;"]
