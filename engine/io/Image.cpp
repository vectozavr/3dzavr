#include <algorithm>
#include <cmath>
#include "linalg/Vec3D.h"
#include "Image.h"
#include <Consts.h>

Image::Image(uint16_t width, uint16_t height) : _width(width), _height(height), _valid(true) {
    if(width != 0 && height != 0) {
        _data = new png_byte[_height * _width * 4];
    } else {
        invalidate();
    }
}

Image::Image(Image&& other) noexcept : _width(other._width), _height(other._height), _valid(other._valid), _data(other._data) {
    other._data = nullptr;
    other.invalidate();
}

Image::Image(const FilePath &filename) {

    FILE *fp = fopen(filename.str().c_str(), "rb");

    if (!fp) {
        fp = fopen(Consts::DEFAULT_TEXTURE_PATH.str().c_str(), "rb");
        if (!fp) {
            invalidate();
            return;
        }
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

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
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

    size_t real = png_get_rowbytes(png, info);
    if (real != 4 * _width) abort();

    _data = new png_byte[_height * _width * 4];
    png_bytepp tmp_rows = new png_bytep[_height];
    for(size_t y = 0; y < _height; y++) {
        tmp_rows[y] = _data + y * _width * 4;
    }
    png_read_image(png, tmp_rows);
    delete[] tmp_rows;

    fclose(fp);

    png_destroy_read_struct(&png, &info, nullptr);
    _valid = true;
}

Image::~Image() {
    invalidate();
}

void Image::invalidate() {
    if (_data != nullptr) {
        delete[] _data;
    }
    _data = nullptr;
    _valid = false;
    _width = _height = 0;
}

Image& Image::operator=(Image&& other) noexcept {
    if (_data != nullptr) delete[] _data;
    _valid = other._valid;
    _data = other._data;
    other._data = nullptr;
    other.invalidate();
    return *this;
}



Image::CODE Image::save2png(const FilePath &file_name, uint16_t bit_depth) {
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

    png_bytepp tmp_rows = new png_bytep[_height];
    for (size_t y = 0; y < _height; y++) {
        tmp_rows[y] = _data + y * _width * 4;
    }

    // Set png info like width, height, bit depth and color type
    // in this example, I assumed grayscale image. You can change image type easily
    png_init_io(png_ptr, fp2);
    png_set_IHDR(png_ptr, info_ptr, _width, _height, bit_depth, \
    PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, \
    PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Write png file
    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, tmp_rows);
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    delete[] tmp_rows;

    return SUCCESS;
}

void Image::set_pixel(uint16_t x, uint16_t y, const Color& color) {
    size_t offset = (y * _width + x) * 4;
    _data[offset + 0] = color.r();
    _data[offset + 1] = color.g();
    _data[offset + 2] = color.b();
    _data[offset + 3] = color.a();
}

inline Color Image::get_pixel_unsafe(uint16_t x, uint16_t y) const {
    size_t offset = (y * _width + x) * 4;
    return Color(_data[offset + 0],
                 _data[offset + 1],
                 _data[offset + 2],
                 _data[offset + 3]);
}

Color Image::get_pixel(uint16_t x, uint16_t y) const {
    // x and y should be in range of the image size
    x = std::min<uint16_t>(_width - 1, x);
    y = std::min<uint16_t>(_height - 1, y);
    return get_pixel_unsafe(x, y);
}

uint16_t repeatClamp(int64_t coord, uint16_t limit) {
    int16_t wrapped = coord % limit;
    if (wrapped < 0) {
        wrapped += limit;
    }
    return wrapped;
};

uint16_t mirrorClamp(int64_t coord, uint16_t limit) {
    if (coord >= 0 && coord < limit) {
        // If the coordinate is within the [0, 1) range, use it as is.
        return coord;
    } else {
        // Calculate the mirrored coordinate
        uint16_t double_limit = limit * 2;
        int16_t mirrored = coord % double_limit;
        if (mirrored < 0) {
            mirrored += double_limit;
        }
        if (mirrored > limit) {
            mirrored = double_limit - mirrored;
        }
        // Clamp the mirrored coordinate to [0, 1) range.
        return std::min<uint16_t>(mirrored, limit - 1);
    }
};

uint16_t clampToEdge(int64_t coord, uint16_t limit) {
    // Clamp the coordinate to the nearest edge in the [0, 1) range
    return std::clamp<uint16_t>(coord, 0, limit - 1);
}

Color Image::get_pixel_from_UV(const Vec2D& uv, CLAMP_MODE mode, bool bottomUp) const {

    int64_t scaledUV[2] = { static_cast<int64_t>(uv.x() * _width), static_cast<int64_t>(uv.y() * _height) };
    uint16_t clampedUV[2];

    switch (mode) {
        case REPEAT:
            clampedUV[0] = repeatClamp(scaledUV[0], _width);
            clampedUV[1] = repeatClamp(scaledUV[1], _height);
            break;
        case MIRRORED_REPEAT:
            clampedUV[0] = mirrorClamp(scaledUV[0], _width);
            clampedUV[1] = mirrorClamp(scaledUV[1], _height);
            break;
        case CLAMP_TO_EDGE:
            clampedUV[0] = clampToEdge(scaledUV[0], _width);
            clampedUV[1] = clampToEdge(scaledUV[1], _height);
            break;
    }

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

        return get_pixel_unsafe(clampedUV[0], _height - 1 - clampedUV[1]);
    } else {
        /*
         * Top-down approach
         * n other systems (like DirectX),
         * V = 0 corresponds to the top edge of the texture image, and
         * V = 1 corresponds to the bottom edge.
         *
         */

        return get_pixel_unsafe(clampedUV[0], clampedUV[1]);
    }
}

Image Image::downSampled() const {
    auto newWidth = std::max<uint16_t>(_width/2, 1);
    auto newHeight = std::max<uint16_t>(_height/2, 1);
    Image newImage = Image(newWidth, newHeight);

    for (size_t y = 0; y < newHeight; y++) {
        for(size_t x = 0; x < newWidth; x++) {
            uint16_t r, g, b, a;
            Color tmpColor = get_pixel(x * 2, y * 2);
            r = tmpColor.r(); g = tmpColor.g(); b = tmpColor.b(); a = tmpColor.a();

            tmpColor = get_pixel(x * 2 + 1, y * 2);
            r += tmpColor.r(); g += tmpColor.g(); b += tmpColor.b(); a += tmpColor.a();

            tmpColor = get_pixel(x * 2, y * 2 + 1);
            r += tmpColor.r(); g += tmpColor.g(); b += tmpColor.b(); a += tmpColor.a();

            tmpColor = get_pixel(x * 2 + 1, y * 2 + 1);
            r += tmpColor.r(); g += tmpColor.g(); b += tmpColor.b(); a += tmpColor.a();

            tmpColor = Color(r / 4, g / 4, b / 4, a / 4);
            newImage.set_pixel(x, y, tmpColor);
        }
    }

    return newImage;
}
