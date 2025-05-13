#include "Visualize.hpp"

#include <graphviz/gvc.h>

#include <cstdio>
#include <cstring>
#include <vector>

#include "Vertex.hpp"

namespace visualize {

namespace {
const char* color_white = "white";
const char* color_lightgray = "lightgray";
const char* color_black = "black";
const char* color_blue = "blue";
const char* color_green = "green";

char* createVertexLabel(const Vertex& vertex, int vertexId) {
  char* buffer = new char[32];
  
  if (vertex.symbol) {
    sprintf(buffer, "%d (%c)", vertexId, vertex.symbol);
  } else {
    sprintf(buffer, "%d", vertexId);
  }

  return buffer;
}

Agnode_t* createNode(Agraph_t* g, int id, const Vertex& v) {
  char node_name[16];
  sprintf(node_name, "%d", id);

  Agnode_t* node = agnode(g, (char*)node_name, 1);
  char* label = createVertexLabel(v, id);

  agsafeset(node, (char*)"label", label, (char*)"");
  agsafeset(node, (char*)"shape", (char*)"circle", (char*)"");
  agsafeset(node, (char*)"style", (char*)"filled", (char*)"");
  agsafeset(node, (char*)"fillcolor",
            v.isTerminal ? (char*)color_white : (char*)color_lightgray,
            (char*)"");

  delete[] label;

  return node;
}

void addEdge(Agraph_t* g, Agnode_t* from, Agnode_t* to, const char* color,
             const char* style, const char* label = nullptr) {
  Agedge_t* edge = agedge(g, from, to, nullptr, 1);

  agsafeset(edge, (char*)"color", (char*)color, (char*)"");
  agsafeset(edge, (char*)"style", (char*)style, (char*)"");
  
  if (label) agsafeset(edge, (char*)"label", (char*)label, (char*)"");
}

}  // namespace

void automatonToDot(Trie& trie, const std::string& filename) {
  GVC_t* gvc = gvContext();
  Agraph_t* g = agopen((char*)"AhoCorasick", Agdirected, nullptr);
  
  agsafeset(g, (char*)"rankdir", (char*)"TB", (char*)"");

  std::vector<Agnode_t*> nodes;
  for (int i = 0; i < trie.getVerticesCount(); ++i) {
    nodes.push_back(createNode(g, i, trie.getVertex(i)));
  }

  for (int i = 0; i < trie.getVerticesCount(); ++i) {
    Vertex& v = trie.getVertex(i);
    for (auto& [sym, next] : v.next) {
      char lbl[2] = {sym, '\0'};
      addEdge(g, nodes[i], nodes[next], color_black, "solid", lbl);
    }
  }

  for (int i = 1; i < trie.getVerticesCount(); ++i) {
    if (trie.getVertex(i).suffixLink != -1) {
      addEdge(g, nodes[i], nodes[trie.getVertex(i).suffixLink], color_blue,
              "dashed");
    }
  }

  for (int i = 0; i < trie.getVerticesCount(); ++i) {
    if (trie.getVertex(i).outputLink != -1) {
      addEdge(g, nodes[i], nodes[trie.getVertex(i).outputLink], color_green,
              "solid");
    }
  }

  char output[256];
  sprintf(output, "%s.png", filename.c_str());
  gvLayout(gvc, g, (char*)"dot");
  gvRenderFilename(gvc, g, (char*)"png", output);

  gvFreeLayout(gvc, g);
  agclose(g);
  gvFreeContext(gvc);
}

}  // namespace visualize