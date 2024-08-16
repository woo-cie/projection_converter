# Projection Converter

This project includes a tool for converting map data between different geodetic projection systems. The projection systems supported can include MGRS, Transverse Mercator, etc.

The conversion details (input and output projection types) are specified in two YAML configuration files.

For example, to convert from MGRS to TransverseMercator projection, you would use configuration files like this:

```yaml
# input.yaml
projector_type: "MGRS"
vertical_datum: "WGS84"
mgrs_grid: "54SUE"
```

```yaml
# output.yaml
projector_type: "TransverseMercator"
vertical_datum: "WGS84"
map_origin:
  latitude: xx
  longitude: yy
  altitude: zz
```

## Dependencies

- PCL (Point Cloud Library) 1.3 or higher
- yaml-cpp
- OpenMP
- GeographicLib
- libxml2 2.12 or higher

## Building the Project

To build the project, use the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

This will create an executable named `PCDConverter`, `Lanelet2Converter` and `StoppointsConverter` in the build directory.

## Usage

To run the program, navigate to the build directory and use the following commands.

### PointCloud Map

```bash
./PCDConverter path_to_input_yaml path_to_output_yaml path_to_input_pcd_file path_to_output_pcd_file
```

### Vector Map

```bash
./Lanelet2Converter path_to_input_yaml path_to_output_yaml path_to_input_osm_file path_to_output_osm_file
```

### Stoppoints

```bash
./StoppointsConverter path_to_input_yaml path_to_output_yaml path_to_input_csv_file path_to_output_csv_file
```

Replace `path_to_input_yaml`, `path_to_output_yaml`, `path_to_input_xx_file`, and `path_to_output_xx_file` with the paths to your input YAML configuration file, output YAML configuration file, and input/output map files, respectively.
