// Copyright (C) 2020-2021 Primate Labs Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the names of the copyright holders nor the names of their
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef IMAGE_IMAGE_H__
#define IMAGE_IMAGE_H__

#include <math.h>
#include <stdint.h>

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace headless {

enum ImageFormat : int {
  kImageFormatNone,
  kImageFormatDefault,
  kImageFormatGrayscale,
  kImageFormatGrayscaleAlpha,
  kImageFormatRGB,
  kImageFormatRGBA
};

// Returns the number of channels for the given image format.
int format_channels(ImageFormat format);

// Returns the image format for the given number of channels.  This function
// will become problematic once there are multiple image formats with the same
// number of channels (e.g., if we add support for BGR images).
ImageFormat channels_format(int channels);

template <typename T = uint8_t>
class Image {
  int width_;
  int height_;
  int channels_;
  ImageFormat format_;
  int stride_;
  T* data_;

public:
  Image()
  : width_(0)
  , height_(0)
  , channels_(0)
  , format_(kImageFormatNone)
  , stride_(0)
  , data_(nullptr)
  {
  }

  Image(int width, int height, ImageFormat format = kImageFormatRGB)
  : width_(width)
  , height_(height)
  , channels_(format_channels(format))
  , format_(format)
  , stride_(width_ * channels_)
  , data_(nullptr)
  {
    data_ = static_cast<T*>(malloc(sizeof(T) * height_ * stride_));
  }

  Image(const Image<T>& other)
  : width_(other.width_)
  , height_(other.height_)
  , channels_(other.channels_)
  , format_(other.format_)
  , stride_(other.stride_)
  {
    data_ = static_cast<T*>(malloc(sizeof(T) * height_ * stride_));
    memcpy(data_, other.data_, sizeof(T) * height_ * stride_);
  }

  Image(Image<T>&& other)
  : width_(other.width_)
  , height_(other.height_)
  , channels_(other.channels_)
  , format_(other.format_)
  , stride_(other.stride_)
  , data_(other.data_)
  {
    other.data_ = nullptr;
  }

  Image& operator=(const Image<T>& other)
  {
    if (width_ != other.width_ || height_ != other.height_ || channels_ != other.channels_
      || format_ != other.format_ || stride_ != other.stride_) {
      clear();
      width_ = other.width_;
      height_ = other.height_;
      channels_ = other.channels_;
      format_ = other.format_;
      stride_ = other.stride_;
      data_ = static_cast<T*>(malloc(height_ * stride_));
    }

    memcpy(data_, other.data_, sizeof(T) * height_ * stride_);

    return *this;
  }

  Image& operator=(Image<T>&& other)
  {
    clear();

    width_ = other.width_;
    height_ = other.height_;
    channels_ = other.channels_;
    format_ = other.format_;
    stride_ = other.stride_;
    data_ = other.data_;

    other.data_ = nullptr;

    return *this;
  }

  ~Image()
  {
    if (data_) {
      free(data_);
      data_ = nullptr;
    }
  }

  // Returns the maximum value of a channel.
  T maximum() const;

  // Returns the width of the image in pixels.
  int width() const
  {
    return width_;
  }

  // Returns the height of the image in pixels.
  int height() const
  {
    return height_;
  }

  // Returns the number of colour channels per pixel.
  int channels() const
  {
    return channels_;
  }

  // Returns the format of the image.
  ImageFormat format() const
  {
    return format_;
  }

  // Returns the row stride in sizeof(T).
  int stride() const
  {
    return stride_;
  }

  // Returns the row stride in bytes.
  int stride_bytes() const
  {
    return stride_ * sizeof(T);
  }

  // Returns the size of the image in bytes.
  int bytes() const
  {
    return height_ * stride_ * sizeof(T);
  }

  // Returns the size of the image in pixels.
  int size() const
  {
    return height_ * stride_;
  }

  // Returns a pointer to the pixel data of the image.
  T* data()
  {
    return data_;
  }

  // Returns a pointer to the pixel data of the image.
  const T* data() const
  {
    return data_;
  }

  T* row(int y)
  {
    return &data_[y * stride_];
  }

  const T* row(int y) const
  {
    return &data_[y * stride_];
  }

  // Returns a pointer to the pixel data at coordinates (x, y).
  inline T* pixel(int x, int y)
  {
    return &data_[y * stride_ + x * channels_];
  }

  // Returns a pointer to the pixel data at coordinates (x, y).
  inline const T* pixel(int x, int y) const
  {
    return &data_[y * stride_ + x * channels_];
  }

  // Removes all pixel data leaving an image with a width and height of 0.
  void clear()
  {
    width_ = 0;
    height_ = 0;
    channels_ = 0;
    format_ = kImageFormatNone;
    stride_ = 0;
    if (data_) {
      free(data_);
      data_ = nullptr;
    }
  }

  // Resets the image to the dimensions (width, height).
  void reset(int width, int height, ImageFormat format = kImageFormatRGB)
  {
    // If the new and old dimensions are the same, then don't re-allocate the
    // image buffer.  This reduces contention around the memory allocator.
    if (width == width_ && height == height_ && format_channels(format) == channels_) {
      return;
    }

    // Clear out the existing image data.
    clear();

    width_ = width;
    height_ = height;
    channels_ = format_channels(format);
    format_ = format;
    stride_ = width_ * channels_;
    data_ = static_cast<T*>(malloc(sizeof(T) * height_ * stride_));
  }

  template <typename Codec>
  bool save(const std::string& filename)
  {
    uint8_t* data = nullptr;
    size_t length = 0;
    Codec::compress(*this, &data, &length);

    std::ofstream ofs;
    ofs.open(filename.c_str(), std::ofstream::out | std::ofstream::binary);
    ofs.write(reinterpret_cast<char*>(data), length);
    ofs.close();

    free(data);

    return !ofs.bad();
  }
};

} // namespace headless

#endif // IMAGE_IMAGE_H__
