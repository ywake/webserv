FROM alpine:latest

WORKDIR /test/

RUN apk add --no-cache --update tini npm curl bash

RUN npm install -g bats

ENTRYPOINT [ "tini", "-vv", "--" ]
CMD [ "tail", "-f" ]