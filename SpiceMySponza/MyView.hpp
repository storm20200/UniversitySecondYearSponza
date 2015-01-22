#pragma once

#if !defined    _MY_VIEW_
#define         _MY_VIEW_


// STL headers.
#include <unordered_map>
#include <memory>


// Engine headers.
#include <SceneModel/SceneModel_fwd.hpp>
#include <tygra/WindowViewDelegate.hpp>
#include <tgl/tgl.h>
#include <Mesh.hpp>


// Forward declarations.
struct Vertex;


/// <summary>
/// A basic enumeration to indicate the type of shader to process/create/use/etc.
/// </summary>
enum class ShaderType : int
{   
    Vertex = 0,
    Fragment = 1
};


/// <summary>
/// Used in creating and rendering of a scene using the Sponza graphics data.
/// </summary>
class MyView final : public tygra::WindowViewDelegate
{
    public:
    
        #pragma region Constructors and destructor

        MyView() = default;
        ~MyView() = default;

        MyView (MyView&& move);
        MyView& operator= (MyView&& move);

        MyView (const MyView& copy) = delete;
        MyView& operator= (const MyView& copy) = delete;

        #pragma endregion

        #pragma region Getters and setters

        void setScene (std::shared_ptr<const SceneModel::Context> scene);

        #pragma endregion

    private:

        #pragma region Window functions

        void windowViewWillStart (std::shared_ptr<tygra::Window> window) override;
        void windowViewDidReset (std::shared_ptr<tygra::Window> window, int width, int height) override;
        void windowViewDidStop (std::shared_ptr<tygra::Window> window) override;    
        void windowViewRender (std::shared_ptr<tygra::Window> window) override;

        #pragma endregion

        #pragma region Utility functions

        /// <summary> Will create the program then compile, attach and link all required shaders together. </summary>
        void buildProgram();

        /// <summary> Retrieves all VAO and VBO ready for the rendering of the scene. </summary>
        void buildMeshData();

        /// <summary> 
        /// Fills a given vector with vertex information which is obtained from the given mesh.
        /// </summary>
        /// <param name="vertices"> An array to be filled with Vertex information. </param>
        /// <param name="mesh"> The mesh to retrieve Vertex data from. </param>
        void assembleVertices (std::vector<Vertex>& vertices, const SceneModel::Mesh& mesh);

        /// <summary>
        /// Constructs the VAO for a mesh based on an interleaved VBO.
        /// </summary>
        /// <param name="mesh"> The mesh to have its' VAO constructed. </param>
        void constructVAO (Mesh& mesh);

        #pragma endregion

        #pragma region Implementation data

        GLuint                                          m_program       { 0 };          //!< The ID of the OpenGL program created and used to draw Sponza.
        float                                           m_aspectRatio   { 0.f };        //!< The calculated aspect ratio of the foreground resolution for the application.

        std::shared_ptr<const SceneModel::Context>      m_scene         { nullptr };    //!< The sponza scene.
        std::unordered_map<SceneModel::MeshId, Mesh>    m_meshes        { };            //!< The collection of meshes which will be used in rendering each mesh in the scene.
        GLuint                                          m_hexTexture    { 0 };          //!< The ID of the hex texture to be drawn on Sponza.

        #pragma endregion
};

#pragma region OpenGL creation

/// <summary> 
/// Compiles a shader from a file located on the machine. 
/// <returns> Returns the OpenGL ID of the compiled shader, 0 means an error occurred. </returns>
/// </summary>
/// <param name="fileLocation"> The location of the shader file. </param>
/// <param name="shader"> The type of shader to compile. </param>
GLuint compileShaderFromFile (const std::string& fileLocation, const ShaderType shader);


/// <summary> 
/// Attaches a shader to the given program. It will also fill the shader with the attributes specified. 
/// </summary>
/// <param name="program"> The ID of the OpenGL program to attach the shader to. </param>
/// <param name="shader"> The ID of the OpenGL shader we will be attaching. </param>
/// <param name="attributes"> An array of attributes to bind to the shader. </param>
void attachShader (const GLuint program, const GLuint shader, const std::vector<GLchar*>& attributes);


/// <summary> 
/// Links all attached shaders together ready for use. 
/// <returns> Returns whether the linking process was successful or not. </returns>
/// </summary>
/// <param name="program"> The ID of the OpenGL program which we will be linking together. </param>
bool linkProgram (const GLuint program);


/// <summary>
/// Generates and fills a VBO with the given data.
/// </summary>
/// <param name="vbo"> The empty VBO which will reflect the value of newly bound buffer. </param>
/// <param name="data"> An array of data to fill the vbo with. </param>
/// <param name="target"> The target buffer type, e.g. GL_ARRAY_BUFFER/GL_ELEMENT_ARRAY_BUFFER. </param>
/// <param name="usage"> The usage parameter of the buffered data, e.g. GL_STATIC_DRAW. </param>
template <typename T> void fillVBO (GLuint& vbo, const std::vector<T>& data, const GLenum target, const GLenum usage)
{
    glGenBuffers (1, &vbo);
    glBindBuffer (target, vbo);
    glBufferData (target, data.size() * sizeof (T), data.data(), usage);
    glBindBuffer (target, 0);
}


/// <summary>
/// Generates and binds a texture buffer from the given file location.
/// </summary>
/// <param name="textureBuffer"> The buffer to fill with the texture data. </param>
/// <param name="fileLocation> The location of the texture file to load. </param>
void bindTexture2D (GLuint& textureBuffer, const std::string& fileLocation);

#pragma endregion

#endif // _MY_VIEW_