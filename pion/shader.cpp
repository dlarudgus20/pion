#include "pch.h"
#include "shader.h"

namespace
{
    std::string read_file(const std::string& path)
    {
        std::string buf;

        int string_size;
        FILE* fp = std::fopen(path.c_str(), "r");

        if (!fp)
            throw std::runtime_error("cannot read file");

        std::fseek(fp, 0, SEEK_END);
        string_size = std::ftell(fp);
        std::rewind(fp);

        buf.resize(string_size + 1);
        std::fread(&buf[0], sizeof(char), string_size, fp);
        buf.pop_back();

        fclose(fp);

        return buf;
    }

    GLuint create_shader(const std::string& path, GLenum type)
    {
        std::string src = read_file(path);
        const char* psrc = src.c_str();

        GLuint shader;
        shader = glCreateShader(type);
        glShaderSource(shader, 1, &psrc, NULL);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            throw shader_error((type == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader"), infoLog);
        }

        return shader;
    }

    shader* s_current = nullptr;
}

shader* shader::get_current()
{
    return s_current;
}

void shader::unuse_shader()
{
    if (s_current)
    {
        glUseProgram(0);
        s_current = nullptr;
    }
}

shader::shader(const std::string& vert, const std::string& frag)
{
    m_vert = create_shader(vert, GL_VERTEX_SHADER);
    m_frag = create_shader(frag, GL_FRAGMENT_SHADER);
    m_program = glCreateProgram();

    glAttachShader(m_program, m_vert);
    glAttachShader(m_program, m_frag);
    glLinkProgram(m_program);

    int success;
    char infoLog[512];
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_program, 512, NULL, infoLog);
        throw shader_error("link phase", infoLog);
    }
}

shader::~shader()
{
    if (s_current == this)
    {
        unuse_shader();
    }
}

void shader::use() const
{
    glUseProgram(m_program);
}
