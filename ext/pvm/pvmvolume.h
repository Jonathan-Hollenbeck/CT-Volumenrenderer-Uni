// (c) by Quynh Ngo

#ifndef PVMVolume_H
#define PVMVolume_H

#include "volumeio.h"
#include <iostream>
#include <string>
#include <assert.h>

/**
 * A utility class that contains all relevant volume properties, fuctionalities.
 */
class PVMVolume 
{
public:
	/*
	 * Constructor from a .pvm file.
	 * @param[in] filename the path to the file. e.g. "path/to/my/file.pvm"
	 */
    PVMVolume();
    PVMVolume(std::string filename);
    ~PVMVolume();
	/**
	 * Print volume information.
	 * @param[in] volume itself.
	 */
    void printPVMVolume();
	/**
	 * returns the scalar value at a grid point (x, y, z) in the volume
	 * the first grid point is (0, 0, 0) while the last one is (width, height, depth)
	 * @param[in] (x, y, z) is the location of the grid point in volume
	 * @return scalar value of the grid point (x, y, z) in array data
	 */
    float GetVoxel(int, int, int);
	/**
	 * return the value of any point (x, y, z) using a interpolation scheme
	 * @param[in] (x, y, z) location of the point
	 * @return the value of the point (x, y, z) using a interpolation scheme
	 */
    float GetValue(float, float, float);
	/**
	 * simple getter for the width of the volume
	 * @return the width of the volume
	 */
    unsigned int getWidth() const;
	/**
	 * simple getter for the height of the volume
	 * @return the height of the volume
	 */
    unsigned int getHeight() const;
	/**
	 * simple getter for the depth of the volume
	 * @return the depth of the volume
	 */
	unsigned int getDepth() const;
	/**
	 * simple getter for the x scale of the volume
	 * @return the x scale of the volume
	 */
    float getScalex() const;
	/**
	 * simple getter for the y scale of the volume
	 * @return the y scale of the volume
	 */
    float getScaley() const;
	/**
	 * simple getter for the z scale of the volume
	 * @return the z scale of the volume
	 */
    float getScalez() const;
	/**
	 * switch through the different interpolation schemes
	 */
	void nextInterpolationScheme();

private:
	// name of the pvm file
	std::string  name;

	// actual data
	float*  m_data;
	
	// volume data dimensions
	unsigned int width; 
	unsigned int height;
	unsigned int depth; 
	unsigned int components;

	// scales of the different dimensions
	float scalex;
	float scaley;
	float scalez;

	// interpolation schemes
	int interpolationScheme;
	float nearestNeighborInterpolation(float x, float y, float z);
	float       trilinearInterpolation(float x, float y, float z);
	float   globalShepardInterpolation(float x, float y, float z);
};

#endif

