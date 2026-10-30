#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <climits>
#include <algorithm>

using namespace std;

//1. edge structure : jo batayega agla landmark kansa hai or kitna door hai
struct Edge{
    string destination;
    int distance; // weight
};

//2. Navigation Router class (OOPs optimization)
class NavigationRouter{
    private:
    //  graph representation using adjacency list : map of source -> list of destination edges
    unordered_map<string, vector<Edge>> graph;

    public:
    //map me naya raasta (route) jodne ke liye function
    void addRoute(string source, string dest, int distance){
        graph[source].push_back({dest, distance});
        graph[dest].push_back({source, distance}); //undirected graph (dono taraf se rasta he)
    }

    //3. core logic : Dijkstra's algorithm implementation
    void findShortestPath(string startCity, string endCity){
        // distance map jo har location ka minimum distance store karega
        unordered_map<string, int> distances;
        // parent map jo shortest path ka rasta track karne me help karega
        unordered_map<string, string> parent;

        // sabhi locations ka initial distance infinite set karo
    for (auto const& pair : graph) {
        distances[pair.first] = INT_MAX;
    }

    // min-heap (priority queue) to get the node with minimum distance always
    //pair structure : <distance , location_name>
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

    //starting point ko initialize karo
    distances[startCity] = 0;
    pq.push({0, startCity});
    parent[startCity] = "START";

    while (!pq.empty()) {
        string current = pq.top().second;
        int current_dist = pq.top().first;
        pq.pop();

        // if we reach final destination , then we can skip forword
        if (current == endCity) break;

        // agar queue se nikli distance purani/badi hei to ignore kar
        if(current_dist > distances[current]) continue;

        //apne neighbors ko explore karo
        for(auto edge : graph[current]){
            int new_dist = current_dist + edge.distance;

            // Relaxation step: agar naya rasta chota he, toh update karo
            if (new_dist < distances[edge.destination]) {
                distances[edge.destination] = new_dist;
                parent[edge.destination] = current;
                pq.push({new_dist, edge.destination});
            }
        }
    }

    // 4. path display logic
    if (distances[endCity] == INT_MAX){
        cout<<"No route found between" <<startCity<< "and" << endCity<<endl;
        return ;
    }

    cout << "\n========================================================"<<endl;
    cout << "Navigation Result from" << startCity << "to" << endCity << endl;
    cout << "=========================================================="<<endl;
    cout << "Total Shortest Distance :" << distances[endCity] << "KM" << endl;

    // parent tree ko reverse traverse karke rasta print karna
    vector<string> path;
    string temp = endCity;
    while(temp != "START") {
        path.push_back(temp);
        temp = parent[temp];
    }
    reverse(path.begin(), path.end());

    cout<< "Optimized Route :";
    for (int i=0; i<path.size(); i++){
        cout << path[i];
        if (i < path.size()-1) cout << "->";
    }
    cout << "\n===========================================================================" << endl;
    }
};

int main (){
    NavigationRouter indoreMap;

    //indore ke mock routing networks (Nodes & Distance in KM)
    indoreMap.addRoute("Rajwada", "Palasia" , 4);
      indoreMap.addRoute("Rajwada", "Bhawarkuan" , 5);
        indoreMap.addRoute("Palasia", "Vijay Nagar" , 6);
          indoreMap.addRoute("Bhawarkuan", "Vijay Nagar" , 12); 
           indoreMap.addRoute("Palasia", "Bhawarkuan" , 3);
             indoreMap.addRoute("Vijay Nagar", "Airport" , 15); 
              indoreMap.addRoute("Rajwada", "Airport" , 8);

              // test case : Find shortest route from Bhawarkuan to Vijay Nagar
              // Direct link 12 KM he, but  Bhawarkuan -> Palasia -> Vijay Nagar sirf (3+6 = 9KM) hai.
              indoreMap.addRoute("Bhawarkuan", "Vijay Nagar" , 12);

              string start = "Bhawarkuan";
              string end = "Vijay Nagar";

              indoreMap.findShortestPath(start , end);

              return 0;
}