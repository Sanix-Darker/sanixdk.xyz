# ----- compile stage ----- #
FROM ubuntu:22.04 as builder

RUN apt update -y --fix-missing && apt install gcc make -y

WORKDIR /app

# Copy code/src files
COPY . .

# Build the ./web app
RUN make compile

# this step is to add headers/footers on  all files
RUN make polish

# We build the html pages and put that in /static
RUN make build

# ----- serve stage ----- #
FROM nginx:stable-alpine3.17-slim as prod

# we copy the nginx conf
COPY ./nginx.conf /etc/nginx/nginx.conf

# we just copy the built web pages
COPY --from=builder /app/public/* /usr/share/nginx/html/
COPY --from=builder /app/public/blogs/* /usr/share/nginx/html/blogs/
# COPY --from=builder /app/public/blogs/* /usr/share/nginx/html/blogs/

# we expose the port 80
EXPOSE 80

# and we serve
ENTRYPOINT ["nginx", "-g", "daemon off;"]
