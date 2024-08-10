#include <GeographicLib/MGRS.hpp>
#include <iomanip>
#include <iostream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>
#include <map_projector/map_projector.hpp>
#include <string>
#include <yaml-cpp/yaml.h>

// Function to draw a progress bar
void drawProgressBar(int len, double percent) {
  std::cout << "Progress: ";
  for (int i = 0; i < len; ++i) {
    if (i < static_cast<int>(len * percent)) {
      std::cout << '=';
    } else {
      std::cout << ' ';
    }
  }
  std::cout << " [" << static_cast<int>(100 * percent) << "%]\r";
  std::cout.flush();
}

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cout << "Usage: ./ProjectionConverter input_yaml output_yaml "
                 "input_pcd output_pcd\n";
    return -1;
  }

  // Parse YAML configuration files
  YAML::Node input_config = YAML::LoadFile(argv[1]);
  YAML::Node output_config = YAML::LoadFile(argv[2]);

  // Load Lanelet2 map file
  auto doc = xmlParseFile(argv[3]);
  if (doc == NULL) {
    std::cerr << "Couldn't read file " << argv[3] << std::endl;
    return -1;
  }

  auto xpath_context = xmlXPathNewContext(doc);
  if (xpath_context == NULL) {
    std::cerr << "Unable to create new XPath context." << std::endl;
    xmlFreeDoc(doc);
    return -1;
  }

  auto xpath_object =
      xmlXPathEvalExpression((const xmlChar *)"/osm/node", xpath_context);
  if (xpath_object == NULL) {
    std::cerr << "Unable to create new XPath context." << std::endl;
    xmlFreeDoc(doc);
    return -1;
  }

  // Define converters
  auto in_map_projector = MapProjector::getMapProjector(input_config);
  auto out_map_projector = MapProjector::getMapProjector(output_config);

  auto nodes = xpath_object->nodesetval;
  auto size = nodes->nodeNr;

  // Convert points
  for (auto i = 0; i < size; i++) {
    auto node = nodes->nodeTab[i];

    auto c = node->children;
    xmlNodePtr local_x_node, local_y_node;
    while (c != NULL) {
      if (!xmlStrcmp(c->name, (const xmlChar *)"tag")) {
        auto k = xmlGetProp(c, (const xmlChar *)"k");
        if (!xmlStrcmp(k, (const xmlChar *)"local_x")) {
          local_x_node = c;
        } else if (!xmlStrcmp(k, (const xmlChar *)"local_y")) {
          local_y_node = c;
        }
      }
      c = c->next;
    }
    auto local_x = xmlGetProp(local_x_node, (const xmlChar *)"v");
    auto local_y = xmlGetProp(local_y_node, (const xmlChar *)"v");

    auto ll = in_map_projector->convertToLatLon(
        Coord{std::stod(std::string(reinterpret_cast<char *>(local_x))),
              std::stod(std::string(reinterpret_cast<char *>(local_y)))});
    auto coord = out_map_projector->convertToCoord(ll);

    xmlSetProp(local_x_node, (const xmlChar *)"v",
               (const xmlChar *)(std::to_string(coord.x).c_str()));
    xmlSetProp(local_y_node, (const xmlChar *)"v",
               (const xmlChar *)(std::to_string(coord.y).c_str()));

    // Update and draw the progress bar
    drawProgressBar(70, static_cast<double>(i + 1) / size);
  }

  std::cout << std::endl;

  // Save converted point cloud to file

  auto ret = xmlSaveFormatFileEnc(argv[4], doc, "UTF-8", 1);
  if (ret < 0) {
    std::cerr << "Couldn't export file " << argv[4] << "\n";
    return -1;
  }
  xmlFreeDoc(doc);
  xmlCleanupParser();
  std::cout << "Vector map projection conversion completed successfully.\n";

  return 0;
}
