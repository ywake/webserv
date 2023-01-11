FROM alpine:latest

RUN apk add --no-cache --update build-base python3 git

RUN mkdir -p /home/user42/
RUN git clone http://github.com/ywake/webserv.git /home/user42/webserv

WORKDIR /home/user42/webserv
RUN make

EXPOSE 4242 4243

ENTRYPOINT [ "./webserv", "config_files/basic.conf" ]