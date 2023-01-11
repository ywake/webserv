FROM alpine:latest

RUN apk add --no-cache --update npm curl bash

RUN npm install -g bats

CMD [ "tail", "-f" ]