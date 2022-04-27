echo ${pwd}
docker image build -t knn-paralel .
docker container run -it -i --rm -v $(pwd):/home/knn knn-paralel:latest