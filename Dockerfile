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

# Recursive copy of the full built site into the nginx root.
# Closes BG-14: collapses the three brittle per-file / per-dir COPYs
# previously listed here (`public/*`, `public/blogs/*`, and a hard-coded
# `public/assets/style.css`) into a single recursive copy. Future renames
# inside public/ (style.css → app.css; new files under assets/, blogs/,
# projects/, components/, etc.) now ship with zero Dockerfile edits.
COPY --from=builder /app/public/ /usr/share/nginx/html/

# we expose the port 80
EXPOSE 80

# and we serve
ENTRYPOINT ["nginx", "-g", "daemon off;"]
