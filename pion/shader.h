#pragma once

class shader_error : public std::runtime_error
{
public:
    shader_error(const std::string& phase, const std::string& cause)
        : m_phase(phase), m_cause(cause), std::runtime_error("error in " + phase + ".\n" + cause)
    {
    }

    const std::string& phase() const noexcept
    {
        return m_phase;
    }
    const std::string& cause() const noexcept
    {
        return m_cause;
    }

private:
    std::string m_phase, m_cause;
};

class shader
{
public:
    static shader* get_current();
    static void unuse_shader();

    shader(const shader&) = delete;
    shader& operator =(const shader&) = delete;

    shader(const std::string& vert, const std::string& frag);
    ~shader();

    void use() const;

    operator GLuint() const
    {
        return m_program;
    }

    GLint getUniformLocation(const char* name)
    {
        GLint loc = glGetUniformLocation(m_program, name);
        if (loc < 0)
            throw std::logic_error("invalid uniform variable name");
        return loc;
    }

    void setUniform1i(const char* name, int i)
    {
        glUniform1i(getUniformLocation(name), i);
    }
    void setUniform1f(const char* name, float f)
    {
        glUniform1f(getUniformLocation(name), f);
    }
    void setUniform3f(const char* name, const glm::vec3& vec3)
    {
        glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(vec3));
    }
    void setUniform4f(const char* name, const glm::vec4& vec4)
    {
        glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(vec4));
    }
    void setUniform4fm(const char* name, const glm::mat4& mat4)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat4));
    }
    void setUniform3fm(const char* name, const glm::mat3& mat3)
    {
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat3));
    }

private:
    GLuint m_vert;
    GLuint m_frag;
    GLuint m_program;
};
