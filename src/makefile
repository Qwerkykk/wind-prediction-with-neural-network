OBJS 	= Cluster/Cluster.o main.o DataVector/DataVector.o Initialization/Initialization.o Assignment/Assignment.o Update/Update.o FileUtils/FileUtils.o HashFunctions/hManhattan.o HashFunctions/hashFunctionManhattan.o ClusterCurve/ClusterCurve.o Curve/Curve.o CurveGridInit/init.o CurveEntry/CurveEntry.o CurveToVector/CurveToVector.o Grid/Grid.o Silhouette/Silhouette.o
SOURCE	= Cluster/Cluster.cpp main.cpp DataVector/DataVector.cpp Initialization/Initialization.cpp Assignment/Assignment.cpp Update/Update.cpp FileUtils/FileUtils.cpp HashFunctions/hManhattan.cpp HashFunctions/hashFunctionManhattan.cpp ClusterCurve/ClusterCurve.cpp Curve/Curve.cpp CurveGridInit/init.cpp CurveEntry/CurveEntry.cpp CurveToVector/CurveToVector.cpp Grid/Grid.cpp Silhouette/Silhouette.cpp
HEADER  = Manhattan.h lshManhattan.h Utils/Utils.h Point/Point.h Utils/DTW.h Utils/Euclidean.h Utils/Position.h Config.h clustering.h Cluster/Cluster.h main.h DataVector/DataVector.h Initialization/Initialization.h Assignment/Assignment.h Update/Update.h FileUtils/FileUtils.h HashFunctions/hManhattan.h HashFunctions/hashFunctionManhattan.h ClusterCurve/ClusterCurve.h Curve/Curve.h CurveGridInit/init.h CurveEntry/CurveEntry.h CurveToVector/CurveToVector.h Grid/Grid.h Silhouette/Silhouette.h
OUT  	= clustering
CC	= g++
FLAGS   = -g -c 


$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

Cluster/Cluster.o : Cluster/Cluster.cpp
	$(CC) $(FLAGS) Cluster/Cluster.cpp -o ./Cluster/Cluster.o

DataVector/DataVector.o: DataVector/DataVector.cpp
	$(CC) $(FLAGS) DataVector/DataVector.cpp -o ./DataVector/DataVector.o

Initialization/Initialization.o: Initialization/Initialization.cpp
	$(CC) $(FLAGS) Initialization/Initialization.cpp -o ./Initialization/Initialization.o

Assignment/Assignment.o: Assignment/Assignment.cpp
	$(CC) $(FLAGS) Assignment/Assignment.cpp -o ./Assignment/Assignment.o

Update/Update.o: Update/Update.cpp
	$(CC) $(FLAGS) Update/Update.cpp -o ./Update/Update.o

FileUtils/FileUtils.o: FileUtils/FileUtils.cpp
	$(CC) $(FLAGS) FileUtils/FileUtils.cpp -o ./FileUtils/FileUtils.o

HashFunctions/hManhattan.o: HashFunctions/hManhattan.cpp
	$(CC) $(FLAGS) HashFunctions/hManhattan.cpp -o ./HashFunctions/hManhattan.o


HashFunctions/hashFunctionManhattan.o: HashFunctions/hashFunctionManhattan.cpp
	$(CC) $(FLAGS) HashFunctions/hashFunctionManhattan.cpp -o ./HashFunctions/hashFunctionManhattan.o


ClusterCurve/ClusterCurve.o: ClusterCurve/ClusterCurve.cpp
	$(CC) $(FLAGS) ClusterCurve/ClusterCurve.cpp -o ./ClusterCurve/ClusterCurve.o


Curve/Curve.o: Curve/Curve.cpp
	$(CC) $(FLAGS) Curve/Curve.cpp -o ./Curve/Curve.o


CurveGridInit/init.o: CurveGridInit/init.cpp
	$(CC) $(FLAGS) CurveGridInit/init.cpp -o ./CurveGridInit/init.o


CurveEntry/CurveEntry.o: CurveEntry/CurveEntry.cpp
	$(CC) $(FLAGS) CurveEntry/CurveEntry.cpp -o ./CurveEntry/CurveEntry.o

CurveToVector/CurveToVector.o: CurveToVector/CurveToVector.cpp
	$(CC) $(FLAGS) CurveToVector/CurveToVector.cpp -o ./CurveToVector/CurveToVector.o


Grid/Grid.o: Grid/Grid.cpp
	$(CC) $(FLAGS) Grid/Grid.cpp -o ./Grid/Grid.o


Silhouette/Silhouette.o: Silhouette/Silhouette.cpp
	$(CC) $(FLAGS)  Silhouette/Silhouette.cpp -o ./Silhouette/Silhouette.o

vector-clustering:
	./$(OUT) -i Dataset/Ex2_Datasets/DataVectors_10_10000x100.csv -o outputFile.txt -c cluster.conf

curve-clustering:
	./$(OUT) -i Dataset/curves_clustering/input_projection6.csv -o outputFile.txt -c cluster.conf

1:
	./clustering -i ../resultfile.csv -o Nodes_128_IxAxY_111_K_4 -c cluster.conf

2:
	./clustering -i ../resultfile.csv -o Nodes_128_IxAxY_112_K_4 -c cluster.conf

3:
	./clustering -i ../resultfile.csv -o Nodes_128_IxAxY_121_K_4 -c cluster.conf

4:
	./clustering -i ../resultfile.csv -o Nodes_128_IxAxY_122_K_4 -c cluster.conf

5:
	./clustering -i ../resultfile.csv -o Nodes_128_IxAxY_211_K_4 -c cluster.conf

6:
	./clustering -i ../resultfile.csv -o Nodes_128_IxAxY_212_K_4 -c cluster.conf

7:
	./clustering -i ../resultfile.csv -o Nodes_128_IxAxY_221_K_4 -c cluster.conf

8:
	./clustering -i ../resultfile.csv -o Nodes_128_IxAxY_222_K_4 -c cluster.conf

9:
	./clustering -i ../new_nn_representations.csv -o Nodes_64_IxAxY_111_K_4 -c cluster.conf
10:
	./clustering -i ../new_nn_representations.csv -o Nodes_64_IxAxY_112_K_4 -c cluster.conf
11:
	./clustering -i ../new_nn_representations.csv -o Nodes_64_IxAxY_121_K_4 -c cluster.conf
12:
	./clustering -i ../new_nn_representations.csv -o Nodes_64_IxAxY_122_K_4 -c cluster.conf
13:
	./clustering -i ../new_nn_representations.csv -o Nodes_64_IxAxY_211_K_4 -c cluster.conf
14:
	./clustering -i ../new_nn_representations.csv -o Nodes_64_IxAxY_212_K_4 -c cluster.conf
15:
	./clustering -i ../new_nn_representations.csv -o Nodes_64_IxAxY_221_K_4 -c cluster.conf
16:
	./clustering -i ../new_nn_representations.csv -o Nodes_64_IxAxY_222_K_4 -c cluster.conf



clean:
	rm -f $(OBJS) $(OUT)
