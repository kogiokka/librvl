#include "../src/rvl.h"
#include <stdio.h>

void rvl_test_write_info()
{
    FILE* file = fopen("test.rvl", "wb");
    RVLInfo_t info = {
        .version = { RVL_VERSION_MAJOR, RVL_VERSION_MINOR },
        .gridType = RVLGridType_Regular,
        .gridUnit = RVLGridUnit_NA,
        .dataFormat = RVLDataFormat_Unsigned,
        .dataDimen = RVLDataDimen_Scalar,
        .bitDepth = RVLBitDepth_8Bits,
        .endian = RVLEndian_Little,
        .resolution = { 300, 400, 500 },
        .voxelSize = { 1.0f, 1.0f, 1.0f },
        .originCoord = { -250.0f, 0.0f, -250.0f },
    };
    RVL_t rvl = {
        .io = file,
    };

    rvl_write_file_sig(&rvl);
    rvl_write_INFO(&rvl, &info);
    rvl_write_END(&rvl);
    fclose(file);
}
