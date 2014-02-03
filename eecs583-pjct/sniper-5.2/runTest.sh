CODE_DIR=/home/ram/583/eecs583-pjct/sniper-5.2 # directory of local Git repos
TEST_DIR=/home/ram/sniper-5.2
RESULTS_DIR=/home/ram/583/eecs583-pjct/sniper-5.2/results
export SNIPER_ROOT=$CODE_DIR
export BENCHMARKS_ROOT=/home/ram/benchmarks
make #
if [ ! -d $RESULTS_DIR ]; then
        mkdir $RESULTS_DIR
fi
cd $TEST_DIR
ulimit -c unlimited
currentTime=$(date +"%Y-%m-%d-%H-%M-%S");
outputFileName=stdout.$1.$currentTime.out;
outputFilePath=$RESULTS_DIR/$outputFileName;
if [ -z "$1" ]
	then
		./run-sniper -d $CODE_DIR -n 4 -c gainestown -- /home/ram/benchmarks/stamp-0.9.10/genome/genome -g256 -s16 -n16384 -t4 > $outputFilePath 2>&1
else
	if [ $1 -eq 1 ] #run genome
	then
		./run-sniper -d $CODE_DIR -n 4 -c gainestown -- /home/ram/benchmarks/stamp-0.9.10/genome/genome -g256 -s16 -n16384 -t4 > $outputFilePath 2>&1
	elif [ $1 -eq 2 ] # run ssca2
	then
		./run-sniper -d $CODE_DIR -n 4 -c gainestown -- /home/ram/benchmarks/stamp-0.9.10/ssca2/ssca2  -s13 -i1.0 -u1.0 -l3 -p3 -t4 > $outputFilePath 2>&1
	elif [ $1 -eq 3 ] # run kmeans
	then
		./run-sniper -d $CODE_DIR -n 4 -c gainestown -- /home/ram/benchmarks/stamp-0.9.10/kmeans/kmeans -m40 -n40 -t0.05 -i /home/ram/benchmarks/stamp-0.9.10/kmeans/inputs/random-n2048-d16-c16.txt -p4 > $outputFilePath 2>&1
	elif [ $1 -eq 4 ] # run intruder
	then
		./run-sniper -d $CODE_DIR -n 4 -c gainestown /home/ram/benchmarks/stamp-0.9.10/intruder/intruder -a10 -l4 -n2038 -s1 -t4 > $outputFilePath 2>&1
	else
		echo "Input args: [1-4]"
	fi
fi
cd $CODE_DIR
newFileName=$RESULTS_DIR/sim.1.$currentTime.out;
if [ ! -z "$1" ]; then
	newFileName=$RESULTS_DIR/sim.$1.$currentTime.out
fi
cp $CODE_DIR/sim.out $newFileName
if [ -z "$2" ]
then
	vi + $outputFilePath
	vi $newFileName
else
	if [ $2 -eq 1 ]
	then
		vi + $outputFilePath
		vi $newFileName
	fi
fi

