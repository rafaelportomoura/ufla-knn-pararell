FROM alpine:3.15.4

RUN apk add build-base

COPY . /home/knn-image/

RUN echo -e "#!/bin/bash \
\
OPT=$1 \
\
case $OPT in \
  -d) file_to_compile=\"readCsv.cpp\";; \
  -f) file_to_compile=$2;; \
esac \
\
echo $file_to_compile \
 \
g++ \"/home/knn-image/src/$file_to_compile\" -fopenmp  \
 \
./a.out" > /home/knn-image/compile.sh
ENTRYPOINT [ "/bin/sh", "/home/knn-image/compile.sh" ] 