#runs all of the available benchmarks 5 times
MAX_BENCHMARKS=4
MAX_TESTS=5
for i in `seq 1 $MAX_BENCHMARKS`
do
	echo "Running benchmark $i of $MAX_BENCHMARKS"
	sh runTests.sh $MAX_TESTS $i
done

