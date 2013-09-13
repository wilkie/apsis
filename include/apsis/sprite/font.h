#ifndef APSIS_SPRITE_FONT_H
#define APSIS_SPRITE_FONT_H

#include "apsis/primitives/texture.h"

#include "apsis/primitives/camera.h"
#include "apsis/primitives/vertex_array.h"
#include "apsis/primitives/vertex_buffer.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace Apsis {
  namespace Sprite {
    /*
     *  This class represents a Font and can be used to draw or query
     *  information about a text.
     */
    class Font {
    public:
      /*
       *  Glyph information.
       */
      struct Glyph {
        /*
         *  The code point this glyph represents.
         */
        int codePoint;

        /*
         *  The index of the glyph in this Font.
         */
        unsigned int index;

        /*
         *  Width of drawn glyph.
         */
        float width;

        /*
         *  Height of drawn glyph.
         */
        float height;

        /*
         *  The horizontal distance from the current pen position to
         *  the left-most edge of the glyph's bounding box.
         */
        float bearingLeft;

        /*
         *  The vertical distance from the current pen position to
         *  the top-most edge of the glyph's bounding box.
         */
        float bearingTop;

        /*
         *  The horizontal distance to increment(for left-to-right writing)
         *  or decrement (for right-to-left writing) the pen position after
         *  a glyph has been rendered when processing text.
         */
        float advanceWidth;

        /*
         *  The vertical distance to increment the pen position after a glyph
         *  has been rendered.
         */
        float advanceHeight;
      };

      /*
       *  Load the given font family or return an existing Font.
       */
      static const Apsis::Sprite::Font& load(const char* family);

      /*
       *  Retrieve an already loaded Font class from its id.
       */
      static const Apsis::Sprite::Font& get(unsigned int id);

      /*
       *  Returns the global, unique identifier for this Font.
       */
      unsigned int id() const;

      /*
       *  Returns the name of the font family for this Font.
       */
      const char* family() const;

      /*
       *  Returns glyph information for the given code point.
       */
      const Glyph& glyph(unsigned int codePoint) const;

      /*
        *  The distance from the baseline to the upper most point.
        */
      float ascent() const;;

      /*
        *  The distance from the baseline to the lower most point.
        */
      float descent() const;

      /*
        *  The distance that must be placed between two lines of text.
        */
      float lineGap() const;

      /*
       *  Draws the given string at the given position.
       */
      void draw(const Primitives::Matrix& projection,
                const Primitives::Camera& camera,
                float x,
                float y,
                const char* string) const;

      /*
       *  Draws the given glyph represented by the given index.
       */
      void draw(const Primitives::Matrix& projection,
                const Primitives::Camera& camera,
                float x,
                float y,
                unsigned int index) const;

    private:
      // Constructor
      Font(const char* family);

      // Keeps track of Fonts system-wide.
      static std::vector<std::string> _ids;
      static std::vector<Apsis::Sprite::Font*> _fonts;

      // Font initialization
      static bool _initialized;
      static void _init();

      float _ascent;
      float _descent;
      float _lineGap;

      // Family
      std::string _family;

      // Font loading
      void _load();
      void _loadGlyphBitmap(unsigned int codePoint) const;

      unsigned int _addGlyph(char character,
                             float x,
                             float y,
                             Glyph& glyph) const;

      // Id
      unsigned int _id;

      // We need to keep a texture for this font. (small)
      // Or we need to keep a vertex buffer. (large)
      // And use a shader to render.
      Apsis::Primitives::Texture* _texture;

      // The entire glyph texture is considered a cache:
      mutable unsigned int _x;
      mutable unsigned int _y;
      mutable unsigned int _line_height;

      // Graphics primitives for storing on gpu and drawing.
      mutable Primitives::VertexArray _vao;
      mutable float* _vertices;
      mutable Primitives::VertexBuffer _vbo;
      mutable unsigned int* _elements;
      mutable Primitives::VertexBuffer _ebo;

      // Glyph cache
      mutable std::vector<Glyph> _glyphs;
      mutable std::unordered_map<unsigned int, unsigned int> _bitmapGlyphs;

      // Internal (FreeType)
      mutable void* _internal;
    };
  }
}

#endif