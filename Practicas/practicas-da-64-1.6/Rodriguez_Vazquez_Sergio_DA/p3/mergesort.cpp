#include <iostream>

int main()
{

}

std::vector<C> merge_sort(std::vector<C> &v, int i, int j)
{
    int n = j - i; 
    if(n <= v.size())
    {
        Insertionsort(v, i, j);
    }
    else
    {
        int k = i - 1 + n/2;
        merge_sort(v, i, k);
        merge_sort(v, k + 1, j);
        merge(v, i, k, j);
    }
    return v;
}

void Insertionsort(std::vector<C> &v, int i, int j)
{
    int key;
    for(i; i < j; i++)
    {
        key = v[i].value;
        j = i - 1;

        while(j >= 0 && v[j].value > key)
        {
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1].value = key;
    }
    
}

void merge(std::vector<C> &v, int i, int k, int j)
{
    int n, p, q;
    n = j - i + 1;
    p = i;
    q = k + 1;
    for(int l = 0; l < n; l++)
    {
        if(p <= k && (q > j || v[p].value <= v[q].value))
        {
            v[l] = v[p];
            p = p + 1;
        }
        else
        {
            v[l] = v[q];
            q = q + 1;
        }
    }
    
    for(int l = 0; l < n; l++)
    {
        v[i - 1 + l] = v[l];
    }
}


void mergesortfunction(bool **freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object *> obstacles, std::list<Defense *> defenses)
{
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;
    float x, y;
    int ultima_colocada = -1;  

    std::vector<C> Candidates;

    for(int i = 0; i < nCellsHeight; i++)
    {
        for(int j = 0; j < nCellsWidth; j++)
        {
            C c({cellCenterToPosition(i, j, cellWidth, cellHeight)}, defaultCellValue(i, j, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses));
            Candidates.push_back(c);
        }
    }  
    Candidates = merge_sort(Candidates, 0, Candidates.size());
}
void mergeSort(int arr[], int l, int r)
{
    if(l < r)
    {
        int m = (l + r) / 2;
        mergeSort(arr, 0, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m ,r);
    }
}


void merge(int arr[], int l, int m, int r)
{
    int i = l; //starting index for left subarray
    int j = m + 1; //starting index for right subarray
    int k = l; //starting index for temporary

    int temp[];

    while(i <= m && j<=r)
    {
        if(arr[i] <= arr[j])
        {
            temp[k] = arr[i]; //arr[i] is smaller than arr[j]
            i++;
            k++;

        }
        else
        {
            temp[k] = arr[j]; //arr[j] is smaller than arr[i]
            j++;
            k++;
        }
    }
    while(i <= m)
    {
        temp[k] == arr[i]; //copying all elements from left subarray to temp as it is
        i++;
        k++;
    }
    while(j <= r)
    {
        temp[k] = arr[j]; //copying all elements from right subarray to temp as it is
        j++;
        k++;
    }
    for(int s = 0; s < 5; s++)
    {
        arr[s] = temp[s];
    }
}