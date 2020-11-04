// (c) by Quynh Ngo

#include "pvmvolume.h"

PVMVolume::PVMVolume()
{

}

PVMVolume::PVMVolume(std::string filename)
{
    name = filename;
	interpolationScheme = 0;

    // load volume and fill properties with values
    unsigned char* tempData = readPVMvolume(filename.c_str(), &width, &height, &depth,
                                &components, &scalex, &scaley, &scalez);

	// init normalized value array
	int size = width*height*depth*components;
	m_data = new float[size];

	// normalize values
	unsigned char maxVal = 0;
	for (int i = 0; i < size; i++)
		maxVal = (tempData[i] > maxVal) ? tempData[i] : maxVal;
	for (int i = 0; i < size; i++)
		m_data[i] = tempData[i] / float(maxVal);

	// delete tempData
	delete tempData;
}

PVMVolume::~PVMVolume()
{
    delete [] m_data;
}

void PVMVolume::printPVMVolume()
{
    if (m_data == nullptr)
	{
		std::cout << "Volume " << name << " wasn't properly loaded" << std::endl;
		std::cout << std::endl;
		return;
	}

	std::cout << "Dataset    " << this->name			<< std::endl;
	std::cout << "Resolution " << this->width  << "x" << this->height << "x" << this->depth << " " << this->components << "byte" << std::endl;
	std::cout << "Scale      " << this->scalex << "x" << this->scaley << "x" << this->scalez << std::endl;
	std::cout << std::endl;
}

float PVMVolume::GetVoxel(int x, int y, int z)
{
    int pos =  (x + y*width + z*width*height)*components + (components-1);
    if( pos > width*height*depth*components || pos < 0) return -1;

    return m_data[pos];
}

float PVMVolume::GetValue(float x, float y, float z)
{
	/* 
	 * x, y, z specify a position within the grid ranging from (0, 0, 0) to (width-1, height-1, depth-1)
	 * The distance between voxels is 1, so (0,0,0), (1,0,0), (10,5,3) are voxel positions but 
	 * e.g. (0.1,0,0), (1.4,0,0), (10,5.1,3) are not. Only at the voxel positions a scalar value is defined. 
	 * This value can be retrieved with getVoxel(x,y,z). If the point (x,y,z) is in between voxel positions 
	 * e.g. (0.1,0,0) an interpolation scheme needs to be utilized the determine the value at this position.
	 *
	 * The grid consists of cells that are defined by 8 neighboring voxels e.g. (4,7,3), (4,7,4), (4,8,3), ..., (5,8,4)
	 * Or generally a grid cell n is enclosed by the 8 surrounding voxel positions from (n,n,n) to (n+1,n+1,n+1).
	 *
	 *     (n,n+1,n+1)---(n+1,n+1,n+1)
	 *        /            /|
	 *       /            / |
	 *      /            /(n+1,n,n+1)
	 *     /            /   /
	 * (n,n+1,n)---(n+1,n+1,n)
	 *    |            |  /
	 *    |            | /
	 * (n,n,n)-----(n+1,n,n)
	 * 
	 *
	 * To get the current cell the position (x,y,z) has to be rounded towards the lower end,
	 * thus the point (5.4, 12.7, 20.4) is in cell (5,12,20). To get the relative normalized 
	 * position within that cell one can calculate: (x - int(x), y - int(y), z - int(z))
	 */

	// apply respective interpolation scheme
	float fInterpolated = 0;
	switch (this->interpolationScheme)
	{
	case 0:
		fInterpolated = nearestNeighborInterpolation(x, y, z);
		break;
	case 1:
		fInterpolated = trilinearInterpolation(x, y, z);
		break;
	case 2:
		fInterpolated = globalShepardInterpolation(x, y, z);
		break;
	}

    return fInterpolated;
}
float PVMVolume::nearestNeighborInterpolation(float x, float y, float z)
{
	// get value of nearest neighbor
	float val = GetVoxel(round(x), round(y), round(z));

	return val;
}
float PVMVolume::trilinearInterpolation(float x, float y, float z)
{
	// get normalized interpolation values in the range [0,1]
	float r = x - (int)x;
	float s = y - (int)y;
	float t = z - (int)z;

	// get values of the 8 neighboring voxels of the current cell
	float f000 = GetVoxel((int)x    , (int)y    , (int)z    );
	float f001 = GetVoxel((int)x    , (int)y    , (int)z + 1);
	float f010 = GetVoxel((int)x    , (int)y + 1, (int)z    );
	float f011 = GetVoxel((int)x    , (int)y + 1, (int)z + 1);
	float f100 = GetVoxel((int)x + 1, (int)y    , (int)z    );
	float f101 = GetVoxel((int)x + 1, (int)y    , (int)z + 1);
	float f110 = GetVoxel((int)x + 1, (int)y + 1, (int)z    );
	float f111 = GetVoxel((int)x + 1, (int)y + 1, (int)z + 1);

	// trilinear interpolation
	float vx1 = (1 - r)*f000 + r * f100;
	float vx2 = (1 - r)*f010 + r * f110;
	float vy1 = (1 - s)*vx1  + s * vx2;

	float vx3 = (1 - r)*f001 + r * f101;
	float vx4 = (1 - r)*f011 + r * f111;
	float vy2 = (1 - s)*vx3  + s * vx4;

	float vz  = (1 - t)*vy1  + t * vy2;

	return vz;
}
float PVMVolume::globalShepardInterpolation(float x, float y, float z)
{
	float  sum = 0;
	float  norm = 0;

	// iterate over all data points
	for (int zi = 0; zi < depth; zi++)
	{
		for (int yi = 0; yi < height; yi++)
		{
			for (int xi = 0; xi < width; xi++)
			{
				// get f(si)
				float fi = GetVoxel(xi, yi, zi);

				// calc |p - pi|^-2
				float dx = x - xi;
				float dy = y - yi;
				float dz = z - zi;
				float dist = sqrt(dx*dx + dy*dy + dz*dz);
				float term = 1.0 / (dist*dist);

				// SUM_[i=1, n] f(si) * |p - pi|^-2
				sum += fi * term;

				// SUM_[j=1, n] |p - pj|^-2
				norm += term;
			}
		}
	}

	// SUM_[i=1, n] (f(si) * |p - pi|^-2)  / (SUM_[j=1, n] |p - pj|^-2) 
	return sum / norm;
}


unsigned int PVMVolume::getWidth() const
{
    return width;
}

unsigned int PVMVolume::getHeight() const
{
    return height;
}

unsigned int PVMVolume::getDepth() const
{
    return depth;
}

float PVMVolume::getScalex() const
{
    return scalex;
}

float PVMVolume::getScaley() const
{
    return scaley;
}

float PVMVolume::getScalez() const
{   
    return scalez;
}

void PVMVolume::nextInterpolationScheme()
{
	this->interpolationScheme++;
	this->interpolationScheme %= 3;
}