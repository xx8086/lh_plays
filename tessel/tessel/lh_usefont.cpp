#include "lh_usefont.h"
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

//////////
namespace ae
{
    void CUseFont::insertfont(std::wstring text)
    {
        // Load character glyph
        std::wstring::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            if (Characters.find(*c) != Characters.end())
            {
                continue;
            }
            
            if (FT_Load_Char(face, *c, FT_LOAD_RENDER))
            {
                //std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // Generate texture
            
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                         GL_TEXTURE_2D,
                         0,
                         GL_RED_EXT,
                         face->glyph->bitmap.width,
                         face->glyph->bitmap.rows,
                         0,
                         GL_RED_EXT,
                         GL_UNSIGNED_BYTE,
                         face->glyph->bitmap.buffer
                         );
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            // Now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (GLuint)face->glyph->advance.x
            };
            Characters.insert(std::pair<wchar_t, Character>(*c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    CUseFont::~CUseFont()
    {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }
    
    void CUseFont::init(std::wstring text, std::string ftt)
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        if (FT_Init_FreeType(&ft))
            return ;
        
        if (FT_New_Face(ft, ftt.c_str(), 0, &face))
            return ;
        
        //FT_Outline_Embolden();
        // Set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        insertfont(text);
        
        glGenVertexArraysOES(1, &_vao);
        glGenBuffers(1, &_vbo);
        glBindVertexArrayOES(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArrayOES(0);
    }
    
    void CUseFont::settext(std::wstring text, GLfloat x, GLfloat y, GLfloat scale, float deep)
    {
        _font_deep = -deep;
        std::wstring::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];
            GLfloat xpos = x + ch.Bearing.x * scale;
            GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;
            
            _vec_font.push_back(FontData(ch.TextureID, xpos, ypos, xpos+w, ypos+h));
            x += (ch.Advance >> 6) * scale;
            
        }
    }
    
    void CUseFont::renderfrone()
    {
        std::vector<tRV> vec_vertices;
        for (VEC_FONT_DATA::const_iterator iter = _vec_font.begin();
            iter != _vec_font.end();
            iter++){
            vec_vertices.clear();
            get_triangles(vec_vertices, iter->_a, iter->_b, iter->_c, iter->_d, 0.0);
            draw_quad(iter->_tid, vec_vertices);
        }
    }
    
    void CUseFont::renderback()
    {
        std::vector<tRV> vec_vertices;
        for (VEC_FONT_DATA::const_iterator iter = _vec_font.begin();
            iter != _vec_font.end();
            iter++){
            vec_vertices.clear();
            get_triangles(vec_vertices, iter->_a, iter->_b, iter->_c, iter->_d, _font_deep);
            draw_quad(iter->_tid, vec_vertices);
        }
    }
    
    void CUseFont::renderside(){
        std::vector<tRV> vec_vertices;
        for (VEC_FONT_DATA::const_iterator iter = _vec_font.begin();
             iter != _vec_font.end();
             iter++){
            vec_vertices.clear();
            get_vertical_triangles_font(vec_vertices, iter->_a, iter->_b, iter->_c);
            draw_quad(iter->_tid, vec_vertices);
            
            vec_vertices.clear();
            get_vertical_triangles_right(vec_vertices, iter->_c, iter->_b, iter->_d);
            draw_quad(iter->_tid, vec_vertices);
            
            vec_vertices.clear();
            get_vertical_triangles_back(vec_vertices, iter->_a, iter->_d, iter->_c);
            draw_quad(iter->_tid, vec_vertices);
            
            vec_vertices.clear();
            get_vertical_triangles_left(vec_vertices, iter->_a, iter->_b, iter->_d);
            draw_quad(iter->_tid, vec_vertices);
        }
    }
    
    void CUseFont::draw_quad(int tid, std::vector<tRV>& vec_vertices)
    {
        glBindTexture(GL_TEXTURE_2D, tid);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(tRV), vec_vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    void CUseFont::get_triangles(std::vector<tRV>& vec_ver, float a, float b, float c, float d, float deep)
    {
        vec_ver.push_back(tRV(a, d, deep, 0.0, 0.0));
        vec_ver.push_back(tRV(a, b, deep, 0.0, 1.0));
        vec_ver.push_back(tRV(c, b, deep, 1.0, 1.0));
        
        vec_ver.push_back(tRV(a, d, deep, 0.0, 0.0));
        vec_ver.push_back(tRV(c, b, deep, 1.0, 1.0));
        vec_ver.push_back(tRV(c, d, deep, 1.0, 0.0));
    }
    
    void CUseFont::get_vertical_triangles_font(std::vector<tRV>& vec_ver, float a, float b, float c)
    {
        vec_ver.push_back(tRV(a, b, 0, 0.0, 0.0));
        vec_ver.push_back(tRV(a, b, _font_deep, 0.0, 1.0));
        vec_ver.push_back(tRV(c, b, _font_deep, 1.0, 1.0));
        
        vec_ver.push_back(tRV(a, b, 0, 0.0, 0.0));
        vec_ver.push_back(tRV(c, b, _font_deep, 1.0, 1.0));
        vec_ver.push_back(tRV(c, b, 0, 1.0, 0.0));
    }
    
    void CUseFont::get_vertical_triangles_back(std::vector<tRV>& vec_ver, float a, float d, float c)
    {
        vec_ver.push_back(tRV(c, d, 0, 0.0, 0.0));
        vec_ver.push_back(tRV(c, d, _font_deep, 0.0, 1.0));
        vec_ver.push_back(tRV(a, d, _font_deep, 1.0, 1.0));
        
        vec_ver.push_back(tRV(c, d, 0, 0.0, 0.0));
        vec_ver.push_back(tRV(a, d, _font_deep, 1.0, 1.0));
        vec_ver.push_back(tRV(a, d, 0, 1.0, 0.0));
    }
    
    void CUseFont::get_vertical_triangles_right(std::vector<tRV>& vec_ver,
                                                float c, float b, float d)
    {
        vec_ver.push_back(tRV(c, b, 0, 0.0, 0.0));
        vec_ver.push_back(tRV(c, b, _font_deep, 0.0, 1.0));
        vec_ver.push_back(tRV(c, d, _font_deep, 1.0, 1.0));
        
        vec_ver.push_back(tRV(c, b, 0, 0.0, 0.0));
        vec_ver.push_back(tRV(c, d, _font_deep, 1.0, 1.0));
        vec_ver.push_back(tRV(c, d, 0, 1.0, 0.0));
    }
    
    void CUseFont::get_vertical_triangles_left(std::vector<tRV>& vec_ver, float a, float b, float d)
    {
        vec_ver.push_back(tRV(a, d, 0, 0.0, 0.0));
        vec_ver.push_back(tRV(a, d, _font_deep, 0.0, 1.0));
        vec_ver.push_back(tRV(a, b, _font_deep, 1.0, 1.0));
        
        vec_ver.push_back(tRV(a, d, 0, 0.0, 0.0));
        vec_ver.push_back(tRV(a, b, _font_deep, 1.0, 1.0));
        vec_ver.push_back(tRV(a, b, 0, 1.0, 0.0));
    }
    
    void CUseFont::renderbegin()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArrayOES(_vao);
    }
    
    void CUseFont::renderend()
    {
        glBindVertexArrayOES(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
}
