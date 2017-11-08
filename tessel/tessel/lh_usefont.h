#pragma once

#include "ft2build.h"
#include "freetype/ftglyph.h"
#include <math.h>
#include <assert.h>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include "../render/shader.h"
#include <vector>

namespace ae
{
    /// Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        GLuint TextureID;   // ID handle of the glyph texture
        glm::ivec2 Size;    // Size of glyph
        glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
        GLuint Advance;    // Horizontal offset to advance to next glyph
    };
    
    
    class   CUseFont
    {
        
    public:
        void init(std::wstring text, std::string ftt);
        ~CUseFont();
        void insertfont(std::wstring text);
        std::map<wchar_t, Character> Characters;
        void settext(std::wstring text, GLfloat x, GLfloat y, GLfloat scale, GLfloat deep);
        void renderfrone();
        void renderback();
        void renderbegin();
        void renderside();
        void renderend();
        
        struct tRV{
            struct POINT{
                float x;
                float y;
                float z;
                POINT(float a, float b, float c):
                x(a), y(b), z(c){};
            } _point;
            struct UV
            {
                float x;
                float y;
                UV(float u, float v):
                x(u), y(v){};
            } _uv;
            
            void set_point(float a, float b, float c){
                _point.x = a;
                _point.y = b;
                _point.z = c;
            }
            void set_uv(float a, float b)
            {
                _uv.x = a;
                _uv.y = b;
            }
            
            tRV(float a, float b, float c, float u, float v):
            _point(a, b, c), _uv(u, v){}
            
        };
    protected:
        void draw_quad(int, std::vector<tRV>&);
        void get_triangles(std::vector<tRV>&, float, float, float, float, float);
        void get_vertical_triangles_font(std::vector<tRV>&, float, float, float);
        void get_vertical_triangles_right(std::vector<tRV>&, float, float, float);
        void get_vertical_triangles_back(std::vector<tRV>&, float, float, float);
        void get_vertical_triangles_left(std::vector<tRV>&, float, float, float);
    private:
        struct FontData
        {
            GLuint _tid;
            GLfloat _a;
            GLfloat _b;
            GLfloat _c;
            GLfloat _d;
            FontData(GLuint tid, GLfloat a, GLfloat b, GLfloat c, GLfloat d):
            _tid(tid), _a(a), _b(b), _c(c), _d(d){};
        } ;
        
        FT_Library ft;
        GLuint texture;
        FT_Face face;
        typedef std::vector<FontData> VEC_FONT_DATA;
        VEC_FONT_DATA _vec_font;
        GLfloat _font_deep = 0.0f;
        GLuint _vao;
        GLuint _vbo;
    };
}
