//
// Created by Ivan Ilin on 21/09/2023.
//

#include <fstream>
#include "linalg/Vec3D.h"
#include "Image.h"

Image::Image(uint16_t width, uint16_t height) : _width(width), _height(height), _valid(true) {

    if(width != 0 && height != 0) {
        _row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * _height);
        for(int y = 0; y < _height; y++) {
            _row_pointers[y] = (png_byte*)malloc(_width*4);
        }
    } else {
        _valid = false;
    }
}

Image::CODE Image::save2png(const FileName &file_name, uint16_t bit_depth) {
    if(!isValid()) {
        return ERROR;
    }

    FILE * fp2 = fopen(file_name.str().c_str(), "wb");
    if (!fp2) {
        return FILE_OPEN_ERROR;
    }

    // Create png struct pointer
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr){
        // dealing with error
        return PNG_STRUCT_ERROR;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        // dealing with error
        return INFO_STRUCT_ERROR;
    }

    // Set png info like width, height, bit depth and color type
    // in this example, I assumed grayscale image. You can change image type easily
    png_init_io(png_ptr, fp2);
    png_set_IHDR(png_ptr, info_ptr, _width, _height, bit_depth, \
    PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, \
    PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Write png file
    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, _row_pointers);
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return SUCCESS;
}

void Image::set_pixel(uint16_t x, uint16_t y, const Color& color) {
    _row_pointers[y][4*x + 0] = color.r();
    _row_pointers[y][4*x + 1] = color.g();
    _row_pointers[y][4*x + 2] = color.b();
    _row_pointers[y][4*x + 3] = color.a();
}

Color Image::get_pixel(uint16_t x, uint16_t y) const {
    // x and y should be in range of the image size
    x = x < 0 ? 0 : x;
    x = x >= _width ? _width-1 : x;
    y = y < 0 ? 0 : y;
    y = y >= _height ? _height-1 : y;

    return Color(_row_pointers[y][4*x + 0],
                 _row_pointers[y][4*x + 1],
                 _row_pointers[y][4*x + 2],
                 _row_pointers[y][4*x + 3]);
}

Image::Image(const FileName &filename) {

    FILE *fp = fopen(filename.str().c_str(), "rb");

    if (!fp) {
        _valid = false;
        return;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                             nullptr, nullptr, nullptr);
    if(!png) abort();

    png_infop info = png_create_info_struct(png);
    if(!info) abort();

    if(setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    png_read_info(png, info);

    _width      = png_get_image_width(png, info);
    _height     = png_get_image_height(png, info);
    auto color_type = png_get_color_type(png, info);
    auto bit_depth  = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if(bit_depth == 16)
        png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    //// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
       color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    _row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * _height);
    for(int y = 0; y < _height; y++) {
        _row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }
    png_read_image(png, _row_pointers);

    fclose(fp);

    png_destroy_read_struct(&png, &info, nullptr);
    _valid = true;
}

Color Image::get_pixel_from_UV(const Vec2D& uv, bool bottomUp) const {

    /*
     * In most systems, U = 0 corresponds to the left edge of the
     * texture image, and U = 1 corresponds to the right edge.
     */

    if(bottomUp) {
        /*
         * Bottom-up approach (used in most systems including Blender)
         * In some systems (like OpenGL),
         * V = 0 corresponds to the bottom edge of the texture image, and
         * V = 1 corresponds to the top edge.
         *
         */

        return get_pixel((uint16_t)(uv.x()*_width), (uint16_t)((1-uv.y())*_height));
    } else {
        /*
         * Top-down approach
         * n other systems (like DirectX),
         * V = 0 corresponds to the top edge of the texture image, and
         * V = 1 corresponds to the bottom edge.
         *
         */

        return get_pixel((uint16_t)(uv.x()*_width), (uint16_t)(uv.y()*_height));
    }
}

Image::~Image() {
    // deallocate memory
    if(_row_pointers != nullptr) {
        for(int y = 0; y < _height; y++) {
            free(_row_pointers[y]);
            _row_pointers[y] = nullptr;
        }
        free(_row_pointers);
        _row_pointers = nullptr;
    }

    _valid = false;
}

std::shared_ptr<Image> Image::downSampled() const {
    auto newWidth = (uint16_t)std::max(_width/2.0, 1.0);
    auto newHeight = (uint16_t)std::max(_height/2.0, 1.0);
    std::shared_ptr<Image> newImage = std::make_shared<Image>(newWidth, newHeight);

    for(int x = 0; x < newWidth; x++) {
        for(int y = 0; y < newHeight; y++) {
            Color sumColor =
                    get_pixel(x*2, y*2) +
                    get_pixel(x*2+1, y*2) +
                    get_pixel(x*2, y*2+1) +
                    get_pixel(x*2+1, y*2+1);
            newImage->set_pixel(x, y, sumColor/4);
        }
    }

    return newImage;
}
