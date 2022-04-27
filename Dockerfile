FROM alpine:3.15.4

RUN apk add build-base

ENTRYPOINT [ "/bin/sh" ] 