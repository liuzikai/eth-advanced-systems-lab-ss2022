./run.sh -g german -w 0 -r 1 -p 1
./run.sh -g actor -w 0 -r 1 -p 1
./run.sh -g comp -w 0 -r 1 -p 1
./run.sh -g google -w 0 -r 1 -p 1
./run.sh -g hep -w 0 -r 1 -p 1
./run.sh -g www -w 0 -r 1 -p 1
./run.sh -g us -w 0 -r 1 -p 1
COMMIT=$(git rev-parse --short HEAD)
./clustered_bar.sh -c $COMMIT -w 0 -r 1 -p 1 -n 1
cd ../data
rm -r $COMMIT*
cd ../RUNEXP