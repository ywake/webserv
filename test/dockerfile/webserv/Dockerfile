FROM alpine:latest

RUN apk add --no-cache --update tini build-base python3 git

RUN mkdir -p /home/user42/
RUN git clone http://github.com/ywake/webserv.git /home/user42/webserv

WORKDIR /home/user42/webserv
RUN make

RUN mkfifo test/volume/fifo && \
	mkfifo test/volume/fifo333 && \
	chmod 333 test/volume/fifo333 && \
	echo "noperm file body" > test/volume/file333 && \
	chmod 333 test/volume/file333

EXPOSE 4242 4243

ENTRYPOINT ["tini", "-vv", "--"]
CMD [ "./webserv", "config_files/basic.conf" ]
