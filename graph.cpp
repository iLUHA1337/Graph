#include "graph.h"
#include <fstream>
Graph::Graph() {
    startVertex = currentVertex = finishVertex = nullptr;
    depthSearch = new DepthFirstSearch(vertices);
    breadthSearch = new BreadthFirstSearch(vertices);
    dijkstrasSearch = new DijkstrasSearchAlgorithm(vertices);
    search = dijkstrasSearch;
}

void Graph::save() {
    std::ofstream out("graph.txt");
    short amountVertices = 0, amountEdges = 0;
    amountVertices = vertices.size();
    for(auto vertex:vertices){
        amountEdges += vertex->connections.size();
    }
    out << amountVertices << " " << amountEdges << "\n";
    for(auto vertex:vertices){
        out << vertex->position.x << " " << vertex->position.y << "\n";
    }
    short counter = 0;
    for(auto vertex:vertices){
        for(ushort c = 0; c < vertex->connections.size(); c++){
            for(ushort i = 0; i < vertices.size(); i++) {
                if(vertex->connections[c] == vertices[i]){
                    out << counter << " " << i << "\n";
                    break;
                }
            }
        }
        counter++;
    }
    out.close();
}

void Graph::load() {
    std::ifstream in("graph.txt");
       if(in.is_open()){
           short amountVertex,amountEdges,x,y;
           double dx,dy;
           in >> amountVertex >> amountEdges;
           for(short c = 0; c < amountVertex; c++){
               in >> dx >> dy;
               addVertex(Point(dx,dy));
           }
           for(short c = 0; c < amountEdges; c++){
               in >> x >> y;
               addEdge(vertices[x],vertices[y]);
           }
           in.close();
       }
}

void Graph::init() {
    if(vertices.size() > 0) {
        if(startVertex == nullptr) {
            startVertex = vertices[0];
            startVertex->lengthMinWay = 0;
        }
        if(finishVertex == nullptr) {
            finishVertex = vertices[vertices.size()-1];
        }
        if(currentVertex == nullptr) {
            currentVertex = startVertex;
        }
        if(startVertex->lengthMinWay != 0) {
            startVertex->lengthMinWay = 0;
        }
    }
}

void Graph::setDFSMode() {
    search = depthSearch;
}

void Graph::setBFSMode()
{
    search = breadthSearch;
}

void Graph::setDSAMode() {
    search = dijkstrasSearch;
}

void Graph::setStartVertex(Vertex *vertex) {
    startVertex = vertex;
}

void Graph::setFinishVertex(Vertex *vertex) {
    finishVertex = vertex;
}

void Graph::oneStep(ushort width, ushort height) {
    init();
    if(currentVertex != finishVertex) {

    }
    if(vertices.size() != 0) {
        currentVertex = search->oneStep(currentVertex,width,height);
    }
}

void Graph::restart() {
    delete depthSearch;
    delete breadthSearch;
    delete dijkstrasSearch;
    depthSearch = new DepthFirstSearch(vertices);
    breadthSearch = new BreadthFirstSearch(vertices);
    dijkstrasSearch = new DijkstrasSearchAlgorithm(vertices);
    search = dijkstrasSearch;
    for(auto& vertex:vertices) {
        vertex->visited = false;
        vertex->lengthMinWay = USHORT_MAX;
    }
    currentVertex = startVertex;
    init();
}

void Graph::clearAll() {
    while(vertices.size() != 0) {
        delVertex(vertices[0]);
    }
}

void Graph::addVertex(Point position) {
    vertices.push_back( new Vertex(position) );
}

void Graph::addEdge(Vertex *vertexOne, Vertex *vertexTwo) {
    vertexOne->connections.push_back(vertexTwo);
}

void Graph::delEdge(Vertex *vertexOne, Vertex *vertexTwo) {
    for(ushort c = 0; c < vertexOne->connections.size(); c++) {
        if(vertexOne->connections[c] == vertexTwo) {
            vertexOne->connections.erase(vertexOne->connections.begin()+c);
        }
    }
    restart();
}

void Graph::delVertex(Vertex *vertex) {
   if(vertex == startVertex) {
        startVertex = nullptr;
   }
   if(vertex == finishVertex) {
       finishVertex = nullptr;
   }
   if(vertex == currentVertex) {
       currentVertex = nullptr;
   }
   for(ushort c = 0; c < vertices.size(); c++) {
       if(vertices[c] == vertex) {
           vertices.erase(vertices.begin()+c);
       }
   }
   for(auto& mVertex:vertices) {
       for(ushort c = 0; c < mVertex->connections.size(); c++) {
           if(mVertex->connections[c] == vertex) {
               mVertex->connections.erase(mVertex->connections.begin()+c);
           }
       }
   }
   delete vertex;
   restart();
}

ushort Graph::getAmountVertices() {
    return vertices.size();
}

Vertex *Graph::getVertex(ushort number) {
    if(vertices.size() > number) {
        return vertices[number];
    } else {
        return nullptr;
    }
}

TYPE Graph::getVertexType(Vertex *vertex) {
    if(vertex == currentVertex) {
        return TYPE::CURRENT;
    }
    if(vertex == startVertex) {
        return TYPE::START;
    }
    if(vertex == finishVertex) {
        return TYPE::FINISH;
    }
    return TYPE::COMMON;
}
