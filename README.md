# Maximum K-core Problem

[TOC]
## Abstract
K-core 在圖論上是一個重要的議題。K-core在圖論上有許多應用，包括large-scale visualization(大規模數據可視化)、graph mining(數據挖掘)。在這個期末作業中，我透過不同方法來實作這個K-core的問題。

## What is K-core
給定一個Undirected Graph $G$，對於$G$的subgraph $G'$，$\forall V\in G',deg(V) \leq k$則稱該subgraph為K-core的graph。在這次的期末專題中，要找到這個$K$的最大值是多少，因此稱作Maximum K-core Problem。

## 題目分析
###    Graph Data Structure
對於Graph在coding上的表示方法應該有兩種，分別是 Adjacency Matrix及Adjacency List，而這兩種data structure分別有甚麼優點及缺點呢?讓我們來分析並決定在這次的Project適合用哪一種的資料結構來表示給定的Graph $G$。
*    Adjacency Matrix
        *    1.需要的記憶體空間較大，倘若該$Graph$ $G$有$n$個$vertex$，則空間複雜度為$\theta(n^2)$
        *    2.將$vertex$加入$Graph$中僅需要$O(1)的時間複雜度$
        *    3.對於要檢測一個$vertex$與一個$vertex$間是否有$edge$存在僅需要$O(1)$的時間複雜度
        *    4.倘若要將一個$vertex$從該$Graph$移除，會需要$O(n)$的時間複雜度
        *    5.若要檢測一個$vertex$的$degree$是多少，會需要$O(n)$的時間複雜度

*    Adjacency List
        *    1.需要的記憶體空間較小，不過將vertex加入Graph的時間複雜度為$O(n^2)$
        *    2.對於要檢測一個$vertex$與一個$vertex$間是否有$edge$存在需要$O(deg(vertex))$的時間複雜度
        *    3.倘若要將$vertex$移除，則需要花費$O(deg(vertex))$的時間複雜度

綜合以上的分析，這次的project會需要對$Graph$刪減$vertex$，及計算$deg(vertex)$，因此我選用的資料結構為Adjacency Matrix來存這個Graph。

## Brute Force

### Procedure
*    1.$\forall V \in G$, find $deg(V)$
*    2.$\forall V$, if $deg(V)<k$, remove it
*    3.repeat Step 1 and 2 until all degree of vertex in $G$ < k

### Implementation
*    Allocate Memory for the Adjacency Matrix
建立了一個長寬皆為1000的二維陣列並將其全部初始化為0，待後續讀檔時將edge加入。而變數型態我設定為bool，因為sizeof(bool) = 1 byte < sizeof(int) = 4 bytes，能夠有效的節省記憶體空間。
```c=
//allocate adjancencyMatrix and initialize
bool **adjacencyMatrix(){
    bool **Matrix = (bool **)malloc(sizeof(bool *) * MAX_SIZE);
    for(int i = 0; i < MAX_SIZE;i++){
        Matrix[i] = (bool *)malloc(sizeof(bool) * MAX_SIZE);
    }
    //init
    for (int i = 0; i < MAX_SIZE;i++){
        for (int j = 0; j < MAX_SIZE;j++){
            Matrix[i][j] = 0;
        }
    }
    return Matrix;
}
```

*    ReadFile
在讀檔的同時記錄下總共有多少個vertex，以方便了解之後要對Adjacency Matrix操作的範圍為何。
```c=
void readfile(bool **Matrix){
    int a, b;
    while(scanf("%d %d",&a, &b)!=EOF){
        VERTEX_COUNT = VERTEX_COUNT > b ? VERTEX_COUNT : b;
        Matrix[a][b] = 1;
        Matrix[b][a] = 1;
    }
    VERTEX_COUNT ++;
}
```
如此一來，我們便可以建立了AdjancencyMatrix，示意圖: ![](https://i.imgur.com/ZqaJljV.png)

*    Degree
建立了AdjancencyMatrix，那麼得到一個$vertex$的$degree$便非常容易了，對於$vertex_i$的degree，我們僅需要沿著第i行或是第i列計算有多少個1即可，這邊選擇的是第i行，因此時間複雜度為$O(n)$，$n=|vertex|$，對於要回傳$Graph$中所有$vertex$的$deg$則需要$O(n^2)$。而為什麼我選擇選擇的是traverse第i行而非第i列呢?因為大部分的programming language大多是row-wise的。對於一個二維陣列，沿著row去讀data跟沿著colume去讀data所花費的時間會因為cache hit rate而有所差異。我選擇沿著row讀是因為這樣子讀取data的cache hit rate會比較高，因此所花費的時間較少。

```c=
int *getDegree(bool **Matrix){
    int *degree = malloc(sizeof(int) *VERTEX_COUNT);
    for (int i = 0; i < VERTEX_COUNT;i++){
        degree[i] = 0;
    }
    // using row-wise access to be a cache friendly function
    for (int i = 0; i < VERTEX_COUNT; i++){
        for (int j = 0; j < VERTEX_COUNT; j++){
            if (Matrix[i][j] == 1){
                degree[i]++;
            }
        }
    }
    return degree;
}
```
如此一來，我們便可以回傳所有vertex的degree，示意圖: ![](https://i.imgur.com/f6lM1K5.png)

*    Delete
在計算K-core的過程中會需要將vertex從graph中移除，因此需要一個function來將vertex移除，而對於AdjancencyMatrix要如何把$vertex_i$從Graph中移除呢?僅需要將AdjancencyMatrix中的第i行及第i列全部設為0即可。因此時間複雜度為$O(n)$。
```c=
bool **deleteVertex(bool **Matrix, int vertextodelete){
    for (int i = 0; i < VERTEX_COUNT;i++){
        Matrix[i][vertextodelete] = 0;
    }
    for (int j = 0; j < VERTEX_COUNT;j++){
        Matrix[vertextodelete][j] = 0;
    }
    return Matrix;
}
```

*    K-core degeneracy
終於進到重頭戲要開始計算K-core了，我先複製先前建立好的AdjancencyMatrix到resultMatrix，然後對檢測是否有degree大於k的vertex，如果有就將其刪掉，直到所有的vertex的degree都小於k之後，返還resultMatrix。注意，到這邊仍然無法確定給定的Graph的Maximum K為何，僅能知道該Graph是否為K-core。
```c=
bool **k_core_degeneracy(int k, bool **Matrix){

    bool **resultMatrix = malloc(sizeof(bool *) * VERTEX_COUNT);
    for (int i = 0; i < VERTEX_COUNT;i++){
        resultMatrix[i] = malloc(sizeof(bool) * VERTEX_COUNT);
    }

    //copy the matrix
    for (int i = 0; i < VERTEX_COUNT;i++){
        for (int j = 0; j < VERTEX_COUNT;j++){
            resultMatrix[i][j] = Matrix[i][j];
        }
    }

    while(1){
        int *degree = getDegree(resultMatrix);

        int count = 0;
        for (int i = 0; i < VERTEX_COUNT;i++){
            if(degree[i]<k && degree[i]!=0){
                count++;
            }
        }

        if(count==0){
            free(degree);
            return resultMatrix;
        }

        for (int i = 0; i <VERTEX_COUNT; i++){
            if (degree[i] < k){
                resultMatrix = deleteVertex(resultMatrix, i);
            }
        }
        free(degree);
    }
    return resultMatrix;
}
```

*    Check Maximum k-core
假如給定的Graph的Maximum K-core的k值為3，則對他在做一次k_core_degeneracy(4)所回傳的resultMatrix的元素會全部為0。我們可以透過這個特徵，決定該圖的Maximum K為何。因此我會需要一個function告訴我回傳的Matrix是否全為0，倘若是，我便知道前一次(k=3)的時候便是答案。
```c=
bool allzero(bool **matrix){
    for (int i = 0; i < VERTEX_COUNT;i++){
        for (int j = 0; j < VERTEX_COUNT;j++){
            if(matrix[i][j]!=0){
                return false;
            }
        }
    }
    return true;
}
```

*    DrivenCode
在這邊可以看到我對一開始的AdjacencyMatrix一直做k-core degeneracy，從k=1開始往上做t，直到回傳的resultMatrix內所有的元素全為0，便可以知道給定的Graph中的Maximum K-Core為t-1，再對原圖做k_core_degeneracy(t-1)即可得到相對應的結果了。
```c=
int main(){
    bool **Matrix = adjacencyMatrix();
    readfile(Matrix);
    bool **resultMatrix;
    for (int i = 1;;i++){
        resultMatrix = k_core_degeneracy(i, Matrix);
        if(allzero(resultMatrix)){
            freeResultMatrix(resultMatrix);
            resultMatrix = k_core_degeneracy(i-1, Matrix);
            printf("%d-core\n", i - 1);
            break;
        }
        else{
            freeResultMatrix(resultMatrix);
        }
    }
    outputformat(resultMatrix);
    freeMatrix(Matrix);
    freeResultMatrix(resultMatrix);
    return 0;
}
```

### 結果
這樣的暴力解法已經足夠讓我pass所有的測資了:
Time:2848
Memory:3772
![](https://i.imgur.com/Mq0yLoN.png)


### 缺點
*    1. 浪費記憶體，其實根本不需要$N*N$的方陣來存Graph。透過觀察，可以知道Undirect Graph的上三角矩陣及下三角矩陣其實是對稱的， 因此僅需要花費一半的空間存放，在traversal AdjacencyMatrix時，也不用找那麼大的範圍，僅需要針對上三角矩陣或是下三角矩陣找即可。

*    2. 這個暴力法在解Maximum K core時，每次都是從最一開始的AdjacencyMatrix開始解，而不是將前一次的結果當成下一次的Input去degeneracy，因此浪費相當多的時間。說白話一點就是我在做k=3的k_core_degeneracy時，是拿最一開始讀檔所建立的AdjacencyMatrix當成輸入，但其實比較有效率的方法應該是拿k=2的k_core_degeneracy所回傳的resultMatrix來當成k=3的輸入。

## Brute Force Optimize:
在這個方法中，我選擇使用AdjacencyMatrix的上三角矩陣，來存東西。為何使用上三角而不使用下三角的原因跟cache hit rate有關，使用上三角矩陣可以減少cache miss發生的機率。因為只使用上三角矩陣，上述的那些function勢必也需要修正，這邊僅提出幾個修改的方面。另外，也針對每次都重頭開始做degeneracy這點進行修正，改為sequential的degeneracy。

*    1.對於兩個for loop traverse AdjacencyMatrix  
inner loop可以少做一半的指令，因此效率會大幅提升，大約提升兩倍。
```c=
Before:
for (int i = 0; i < VERTEX_COUNT;i++){
    for (int j = 0; j < VERTEX_COUNT; j++){

    }
}

After:
for (int i = 0; i < VERTEX_COUNT;i++){
    for (int j = i + 1; j < VERTEX_COUNT; j++){

    }
}
```
*    2.Degree  
改成只使用上三角矩陣之後，在計算每個vertex的degree的時候，困擾了我一陣子。究竟要如何計算每個vertex的degree呢?其實很簡單當traverse $AdjacencyMatrix[i][j]$不為0時，表示$vertex_i$與$vertex_j$之間有edge相連，換句話說，也就是$degree[i]+1$及$degree[j]+1$
```c=
int *getDegree(bool **Matrix){
    int *degree = malloc(sizeof(int) * VERTEX_COUNT);
    for (int i = 0; i < VERTEX_COUNT;i++){
        degree[i] = 0;
    }
    // using row-wise access to be a cache friendly function
    for (int i = 0; i < VERTEX_COUNT; ++i){
        for (int j = i+1; j <VERTEX_COUNT; ++j){
            if (Matrix[i][j] == 1){
                degree[i]++;
                degree[j]++;
            }
        }
    }
    return degree;
}
```
*    3.Sequential_degeneracy  
sequential degeneracy做的事情其實跟前面所提到的degeneracy一樣，只是將輸入改為前一層的輸出，如此一來便不用重頭開始算。
```c=
bool **sequential_degeneracy(int k, bool **resultMatrix){
    for(;;){
        int *degree = getDegree(resultMatrix);

        int count = 0;
        for (int i = 0; i < VERTEX_COUNT;++i){
            if(degree[i]<k && degree[i]!=0){
                count++;
            }
        }

        if(count==0){
            free(degree);
            return resultMatrix;
        }

        for (int i = 0; i < VERTEX_COUNT; ++i){
            if (degree[i] < k){
                resultMatrix = deleteVertex(resultMatrix, i);
            }
        }
        free(degree);
    }
    return resultMatrix;
}
```

### 結果
Time:1160
Memory:3820
![](https://i.imgur.com/2J3KDk3.png)
較前一次Brute force的解法進步了不少。

### 缺點
*    1.每次都還是會去檢查那些先前已經被刪除的vertex的degree，如果能做一個Linkedlist去存剩餘那些vertex仍在Graph中，之後在traverse的時候，沿著linkedlist去尋找即可，便不用浪費時間去計算那些已經被刪除的vertex了。我有試著要用linkedlist製作類似look up table的東西，使我不用traversal整個AdjacencyMatrix，而只要去看linkedlist中所存那些vertex的degree即可。但是因為我這個程式一直找不到bug，所以就不放出來獻醜了，僅分享或許這是一個可以再更進一步優化的地方。

*    2.有些工作，其實不用一個一個做，不會因為順序顛倒而產生錯誤，或許可以使用Parallel Programming使用多顆CPU來加速運算。

## ParallelOptimize
雖然FormosaOJ僅開放使用一顆CPU運算，因此我如果進行平行化的程式設計在上面是看不到任何差異的，因此我決定在我的電腦上測試看看使用平行化程式設計是否會影響效能。但是題目提供的側資運算時間實在太少了，因此我到[Test Case Generator](https://test-case-generator.herokuapp.com/)產生比較大的側資讓我能夠比較出他們的差異。而我所使用的testcase也都有push到github上，也會在下面提供連結，供參考。在平行化上面，我使用的是GNU內部的OpenMP來使用多顆CPU運算。針對for loop能夠有效的加速。因為平行化之後的code比較複雜，因此我就不完整放上來了，僅貼上我放在github上的[網址](https://github.com/coherent17/Maximum-K_core/blob/main/Compare/ParallelOptimize.c)

### 結果
測試環境:
我的測試環境是有8顆CPU給我做分配的，為了簡單起見，我這次最多只有使用到4顆CPU。
![](https://i.imgur.com/IA9zXxD.png)

#### [testcase1](https://github.com/coherent17/Maximum-K_core/blob/main/Compare/1000V100E.in): 1000 vertex, 100 edge

|      Using CPU      | 1     | 2     | 3     |   4   |
|:-------------------:| ----- | ----- | ----- |:-----:|
| Execution Time(sec) | 0.061 | 0.033 | 0.023 | 0.019 |

Data Visualization:
![](https://i.imgur.com/VfBiQL2.png)


#### [testcase2](https://github.com/coherent17/Maximum-K_core/blob/main/Compare/1000V1000E.in): 1000 vertex, 1000 edge
|      Using CPU      | 1     | 2     | 3     |   4   |
|:-------------------:| ----- | ----- | ----- |:-----:|
| Execution Time(sec) | 0.150 | 0.063 | 0.049 | 0.043 |

![](https://i.imgur.com/7FPrsoX.png)


#### [testcase3](https://github.com/coherent17/Maximum-K_core/blob/main/Compare/1000V10000E.in): 1000 vertex, 10000 edge
|      Using CPU      | 1     | 2     | 3     |   4   |
|:-------------------:| ----- | ----- | ----- |:-----:|
| Execution Time(sec) | 0.171 | 0.095 | 0.087 | 0.084 |

![](https://i.imgur.com/zjMluFp.png)


#### [testcase4](https://github.com/coherent17/Maximum-K_core/blob/main/Compare/1000V100000E.in): 1000 vertex 100000 edge
|      Using CPU      | 1     | 2     | 3     |   4   |
|:-------------------:| ----- | ----- | ----- |:-----:|
| Execution Time(sec) | 0.450 | 0.388 | 0.398 | 0.393 |

![](https://i.imgur.com/sRZRULi.png)
如果想要在自己的環境跑我的平行化程式碼可以透過在terminal輸入:
```bash=
git clone https://github.com/coherent17/Maximum-K_core #clone program &testcase 
cd Compare
sudo apt install make # first time use make only
make # compile the code
make do1 # execute testcase1
make do2 # execute testcase2
make do3 # execute testcase3
make do4 # execute testcase4
make clean # delete the execute file
```
### 結果分析
![](https://i.imgur.com/uPIfDOT.png)
*    1.在同樣vertex的情況下，edge數越多則需要花費的時間越多。
*    2.使用CPU越多，花費時間越少
*    3.edge數越多，使用平行運算所提升的效能(performance)越少

## 心得
這次的期末project花了我好多時間，雖然在formosaOJ上面的表現沒有非常好，但是我成功透過平行運算將程式的效能大幅提升。透過這個project，讓我對graph這個資料結構更加熟悉，並且也學會如何分析function的時間複雜度以決定要以何種資料結構來存Graph。這次final project所有的code我都有push至我的github帳號留存，網址如下:https://github.com/coherent17/Maximum-K_core