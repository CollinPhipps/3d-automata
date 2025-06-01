#include "Renderer.h"

#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "):" << function <<
            " " << file << ":" << line << '\n';
        return false;
    }
    return true;
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.bind();
    va.bind();
    ib.bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::drawInstanced(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, unsigned int count) const
{
    shader.bind();
    va.bind();
    ib.bind();
    GLCall(glDrawElementsInstanced(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr, count));
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::drawLines(const VertexArray& va, const Shader& shader, unsigned int count) const
{
    shader.bind();
    va.bind();
    glDrawArrays(GL_LINES, 0, count);
}