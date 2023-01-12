FROM alpine:3.15.5

EXPOSE 80

RUN apk add --update --no-cache apache2

COPY ./httpd.conf /etc/apache2/httpd.conf

ENTRYPOINT ["tail", "-f"]
