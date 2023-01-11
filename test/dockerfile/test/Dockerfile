FROM bats/bats:latest

WORKDIR /test/

RUN apk add --no-cache --update curl

ENTRYPOINT ["/tini", "--", "tail", "-f"]