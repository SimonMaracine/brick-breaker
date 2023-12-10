#pragma once

#include <memory>
#include <vector>

namespace bb {
    struct PostProcessingContext;
    class Framebuffer;
    class Shader;
    class Renderer;

    class PostProcessingStep {
    public:
        PostProcessingStep(std::shared_ptr<Framebuffer> framebuffer, std::shared_ptr<Shader> shader)
            : framebuffer(framebuffer), shader(shader) {}
        virtual ~PostProcessingStep() = default;

        PostProcessingStep(const PostProcessingStep&) = delete;
        PostProcessingStep& operator=(const PostProcessingStep&) = delete;
        PostProcessingStep(PostProcessingStep&&) = delete;
        PostProcessingStep& operator=(PostProcessingStep&&) = delete;

        virtual void render(const PostProcessingContext& context) const = 0;
        virtual void prepare(const PostProcessingContext& context) const = 0;
    protected:
        std::shared_ptr<Framebuffer> framebuffer;
        std::shared_ptr<Shader> shader;

        friend class Renderer;
    };

    struct PostProcessingContext {
        std::vector<std::unique_ptr<PostProcessingStep>> steps;
        unsigned int last_texture {0};  // Last texture at any moment in the processing pipeline
        std::vector<unsigned int> textures;  // All textures in order
        unsigned int original_texture {0};
    };
}
