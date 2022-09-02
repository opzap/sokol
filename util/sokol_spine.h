#if defined(SOKOL_IMPL) && !defined(SOKOL_SPINE_IMPL)
#define SOKOL_SPINE_IMPL
#endif
#ifndef SOKOL_SPINE_INCLUDED
/*
    sokol_spine.h -- a sokol-gfx renderer for the spine-c runtime
                     (see https://github.com/EsotericSoftware/spine-runtimes/tree/4.1/spine-c)

    Project URL: https://github.com/floooh/sokol

    Do this:
        #define SOKOL_IMPL or
        #define SOKOL_SPINE_IMPL

    before you include this file in *one* C or C++ file to create the
    implementation.

    The following defines are used by the implementation to select the
    platform-specific embedded shader code (these are the same defines as
    used by sokol_gfx.h and sokol_app.h):

    SOKOL_GLCORE33
    SOKOL_GLES2
    SOKOL_GLES3
    SOKOL_D3D11
    SOKOL_METAL

    ...optionally provide the following macros to override defaults:

    SOKOL_ASSERT(c)     - your own assert macro (default: assert(c))
    SOKOL_SPINE_API_DECL    - public function declaration prefix (default: extern)
    SOKOL_API_DECL      - same as SOKOL_SPINE_API_DECL
    SOKOL_API_IMPL      - public function implementation prefix (default: -)
    SOKOL_LOG(msg)      - your own logging function (default: puts(msg))
    SOKOL_UNREACHABLE() - a guard macro for unreachable code (default: assert(false))

    If sokol_spine.h is compiled as a DLL, define the following before
    including the declaration or implementation:

    SOKOL_DLL

    On Windows, SOKOL_DLL will define SOKOL_SPINE_API_DECL as __declspec(dllexport)
    or __declspec(dllimport) as needed.

    Include the following headers before including sokol_spine.h:

        sokol_gfx.h

    Include the following headers before include the sokol_spine.h *IMPLEMENTATION*:

        spine/spine.h

    FIXME FIXME FIXME
    =================

    LICENSE
    =======
    zlib/libpng license

    Copyright (c) 2022 Andre Weissflog

    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the
    use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

        1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software in a
        product, an acknowledgment in the product documentation would be
        appreciated but is not required.

        2. Altered source versions must be plainly marked as such, and must not
        be misrepresented as being the original software.

        3. This notice may not be removed or altered from any source
        distribution.
*/
#define SOKOL_SPINE_INCLUDED (1)
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h> // size_t

#if !defined(SOKOL_GFX_INCLUDED)
#error "Please include sokol_gfx.h before sokol_spine.h"
#endif

#if defined(SOKOL_API_DECL) && !defined(SOKOL_SPINE_API_DECL)
#define SOKOL_SPINE_API_DECL SOKOL_API_DECL
#endif
#ifndef SOKOL_SPINE_API_DECL
#if defined(_WIN32) && defined(SOKOL_DLL) && defined(SOKOL_SPINE_IMPL)
#define SOKOL_SPINE_API_DECL __declspec(dllexport)
#elif defined(_WIN32) && defined(SOKOL_DLL)
#define SOKOL_SPINE_API_DECL __declspec(dllimport)
#else
#define SOKOL_SPINE_API_DECL extern
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum {
    SSPINE_INVALID_ID = 0,
};

typedef struct sspine_context { uint32_t id; } sspine_context;
typedef struct sspine_atlas { uint32_t id; } sspine_atlas;
typedef struct sspine_skeleton { uint32_t id; } sspine_skeleton;
typedef struct sspine_instance { uint32_t id; } sspine_instance;

typedef struct sspine_atlas_page { sspine_atlas atlas; int index; } sspine_atlas_page;
typedef struct sspine_bone { sspine_instance instance; int index; } sspine_bone;
typedef struct sspine_slot { sspine_instance instance; int index; } sspine_slot;
typedef struct sspine_anim { sspine_instance instance; int index; } sspine_anim;

typedef struct sspine_range { const void* ptr; size_t size; } sspine_range;
typedef struct sspine_vec2 { float x, y; } sspine_vec2;
typedef sg_color sspine_color;

typedef enum SSPINE_resource_state {
    SSPINE_RESOURCESTATE_INITIAL,
    SSPINE_RESOURCESTATE_ALLOC,
    SSPINE_RESOURCESTATE_VALID,
    SSPINE_RESOURCESTATE_FAILED,
    SSPINE_RESOURCESTATE_INVALID,
    _SSPINE_RESOURCESTATE_FORCE_U32 = 0x7FFFFFFF
} sspine_resource_state;

typedef struct sspine_layer_transform {
    sspine_vec2 size;
    sspine_vec2 origin;
} sspine_layer_transform;

typedef struct sspine_bone_transform {
    sspine_vec2 position;
    float rotation;         // in degrees
    sspine_vec2 scale;
    sspine_vec2 shear;      // in degrees
} sspine_bone_transform;

typedef struct sspine_context_desc {
    int max_vertices;
    int max_commands;
    sg_pixel_format color_format;
    sg_pixel_format depth_format;
    int sample_count;
    sg_color_mask color_write_mask;
} sspine_context_desc;

typedef struct sspine_image_info {
    sg_image image;
    const char* filename;
    sg_filter min_filter;
    sg_filter mag_filter;
    sg_wrap wrap_u;
    sg_wrap wrap_v;
    int width;
    int height;
    bool premul_alpha;
} sspine_image_info;

typedef struct sspine_atlas_overrides {
    sg_filter min_filter;
    sg_filter mag_filter;
    sg_wrap wrap_u;
    sg_wrap wrap_v;
    bool premul_alpha_enabled;
    bool premul_alpha_disabled;
} sspine_atlas_overrides;

typedef struct sspine_atlas_desc {
    sspine_range data;
    sspine_atlas_overrides override;
} sspine_atlas_desc;

typedef struct sspine_atlas_page_info {
    sspine_atlas atlas;
    sg_image image;
    const char* name;
    sg_filter min_filter;
    sg_filter mag_filter;
    sg_wrap wrap_u;
    sg_wrap wrap_v;
    int width;
    int height;
    bool premul_alpha;
    sspine_atlas_overrides overrides;
} sspine_atlas_page_info;

typedef struct sspine_skeleton_desc {
    sspine_atlas atlas;
    float prescale;
    float anim_default_mix;
    const char* json_data;
    sspine_range binary_data;
} sspine_skeleton_desc;

typedef struct sspine_bone_info {
    const char* name;
    int index;
    float length;
    sspine_bone_transform pose_tform;
    sspine_bone_transform cur_tform;
    sspine_color color;
} sspine_bone_info;

typedef struct sspine_instance_desc {
    sspine_skeleton skeleton;
} sspine_instance_desc;

typedef struct sspine_allocator {
    void* (*alloc)(size_t size, void* user_data);
    void (*free)(void* ptr, void* user_data);
    void* user_data;
} sspine_allocator;

typedef struct sspine_desc {
    int max_vertices;
    int max_commands;
    int context_pool_size;
    int atlas_pool_size;
    int skeleton_pool_size;
    int instance_pool_size;
    sg_pixel_format color_format;
    sg_pixel_format depth_format;
    int sample_count;
    sg_color_mask color_write_mask;
    sspine_allocator allocator;
} sspine_desc;

// setup/shutdown
SOKOL_SPINE_API_DECL void sspine_setup(const sspine_desc* desc);
SOKOL_SPINE_API_DECL void sspine_shutdown(void);

// context functions
SOKOL_SPINE_API_DECL sspine_context sspine_make_context(const sspine_context_desc* desc);
SOKOL_SPINE_API_DECL void sspine_destroy_context(sspine_context ctx);
SOKOL_SPINE_API_DECL void sspine_set_context(sspine_context ctx);
SOKOL_SPINE_API_DECL sspine_context sspine_get_context(void);
SOKOL_SPINE_API_DECL sspine_context sspine_default_context(void);

// mark a new frame (call at start of a frame before any sspine_draw_instance())
SOKOL_SPINE_API_DECL void sspine_new_frame(void);

// update instance animations before drawing
SOKOL_SPINE_API_DECL void sspine_update_instance(sspine_instance instance, float delta_time);

// draw instance into current or explicit context
SOKOL_SPINE_API_DECL void sspine_draw_instance_in_layer(sspine_instance instance, int layer);
SOKOL_SPINE_API_DECL void sspine_context_draw_instance_in_layer(sspine_context ctx, sspine_instance instance, int layer);

// draw a layer in current context or explicit context (call once per context and frame in sokol-gfx pass)
SOKOL_SPINE_API_DECL void sspine_draw_layer(int layer, const sspine_layer_transform* tform);
SOKOL_SPINE_API_DECL void sspine_context_draw_layer(sspine_context ctx, int layer, const sspine_layer_transform* tform);

// create and destroy spine objects
SOKOL_SPINE_API_DECL sspine_atlas sspine_make_atlas(const sspine_atlas_desc* desc);
SOKOL_SPINE_API_DECL sspine_skeleton sspine_make_skeleton(const sspine_skeleton_desc* desc);
SOKOL_SPINE_API_DECL sspine_instance sspine_make_instance(const sspine_instance_desc* desc);
SOKOL_SPINE_API_DECL void sspine_destroy_atlas(sspine_atlas atlas);
SOKOL_SPINE_API_DECL void sspine_destroy_skeleton(sspine_skeleton skeleton);
SOKOL_SPINE_API_DECL void sspine_destroy_instance(sspine_instance instance);

// get current resource state (INITIAL, ALLOC, VALID, FAILED, INVALID)
SOKOL_SPINE_API_DECL sspine_resource_state sspine_get_context_resource_state(sspine_context context);
SOKOL_SPINE_API_DECL sspine_resource_state sspine_get_atlas_resource_state(sspine_atlas atlas);
SOKOL_SPINE_API_DECL sspine_resource_state sspine_get_skeleton_resource_state(sspine_skeleton skeleton);
SOKOL_SPINE_API_DECL sspine_resource_state sspine_get_instance_resource_state(sspine_instance instance);

// shortcut for sspine_get_*_state() == SSPINE_RESOURCESTATE_VALID
SOKOL_SPINE_API_DECL bool sspine_context_valid(sspine_context context);
SOKOL_SPINE_API_DECL bool sspine_atlas_valid(sspine_atlas atlas);
SOKOL_SPINE_API_DECL bool sspine_skeleton_valid(sspine_skeleton skeleton);
SOKOL_SPINE_API_DECL bool sspine_instance_valid(sspine_instance instance);

// atlas images
SOKOL_SPINE_API_DECL int sspine_num_images(sspine_atlas atlas);
SOKOL_SPINE_API_DECL sspine_image_info sspine_get_image_info(sspine_atlas atlas, int image_index);

// instance transform functions
SOKOL_SPINE_API_DECL void sspine_set_position(sspine_instance instance, sspine_vec2 position);
SOKOL_SPINE_API_DECL void sspine_set_scale(sspine_instance instance, sspine_vec2 scale);
SOKOL_SPINE_API_DECL void sspine_set_color(sspine_instance instance, sspine_color color);
SOKOL_SPINE_API_DECL sspine_vec2 sspine_get_position(sspine_instance instance);
SOKOL_SPINE_API_DECL sspine_vec2 sspine_get_scale(sspine_instance instance);
SOKOL_SPINE_API_DECL sspine_color sspine_get_color(sspine_instance instance);

// instance animation functions
SOKOL_SPINE_API_DECL sspine_anim sspine_find_anim(sspine_instance instance, const char* name);
SOKOL_SPINE_API_DECL void sspine_clear_animation_tracks(sspine_instance instance);
SOKOL_SPINE_API_DECL void sspine_clear_animation_track(sspine_instance instance, int track_index);
SOKOL_SPINE_API_DECL void sspine_set_animation(sspine_instance instance, int track_index, sspine_anim anim, bool loop);
SOKOL_SPINE_API_DECL void sspine_set_animation_by_name(sspine_instance instance, int track_index, const char* anim_name, bool loop);
SOKOL_SPINE_API_DECL void sspine_add_animation(sspine_instance instance, int track_index, sspine_anim anim, bool loop, float delay);
SOKOL_SPINE_API_DECL void sspine_add_animation_by_name(sspine_instance instance, int track_index, const char* anim_name, bool loop, float delay);
SOKOL_SPINE_API_DECL void sspine_set_empty_animation(sspine_instance instance, int track_index, float mix_duration);
SOKOL_SPINE_API_DECL void sspine_add_empty_animation(sspine_instance instance, int track_index, float mix_duration, float delay);

// atlas page functions
SOKOL_SPINE_API_DECL int sspine_num_atlas_pages(sspine_atlas atlas);
SOKOL_SPINE_API_DECL sspine_atlas_page sspine_atlas_page_at(sspine_atlas atlas, int index);
SOKOL_SPINE_API_DECL sspine_atlas_page_info sspine_get_atlas_page_info(sspine_atlas_page page);

// bone functions
SOKOL_SPINE_API_DECL sspine_bone sspine_find_bone(sspine_instance instance, const char* name);
SOKOL_SPINE_API_DECL bool sspine_bone_valid(sspine_bone bone);
SOKOL_SPINE_API_DECL int sspine_num_bones(sspine_instance instance);
SOKOL_SPINE_API_DECL sspine_bone sspine_bone_at(sspine_instance instance, int index);
SOKOL_SPINE_API_DECL sspine_bone_info sspine_get_bone_info(sspine_bone bone);
SOKOL_SPINE_API_DECL void sspine_set_bone_transform(sspine_bone bone, const sspine_bone_transform* transform);
SOKOL_SPINE_API_DECL void sspine_set_bone_position(sspine_bone bone, sspine_vec2 position);
SOKOL_SPINE_API_DECL void sspine_set_bone_rotation(sspine_bone bone, float rotation);
SOKOL_SPINE_API_DECL void sspine_set_bone_scale(sspine_bone bone, sspine_vec2 scale);
SOKOL_SPINE_API_DECL void sspine_set_bone_shear(sspine_bone bone, sspine_vec2 shear);
SOKOL_SPINE_API_DECL sspine_bone_transform sspine_get_bone_transform(sspine_bone bone);
SOKOL_SPINE_API_DECL sspine_vec2 sspine_get_bone_position(sspine_bone bone);
SOKOL_SPINE_API_DECL float sspine_get_bone_rotation(sspine_bone bone);
SOKOL_SPINE_API_DECL sspine_vec2 sspine_get_bone_scale(sspine_bone bone);
SOKOL_SPINE_API_DECL sspine_vec2 sspine_get_bone_shear(sspine_bone bone);

// slot functions
SOKOL_SPINE_API_DECL sspine_slot sspine_find_slot(sspine_instance instance, const char* name);
SOKOL_SPINE_API_DECL int sspine_num_slots(sspine_instance instance);
SOKOL_SPINE_API_DECL sspine_slot sspine_slot_at(sspine_instance instance, int index);
SOKOL_SPINE_API_DECL void sspine_slot_set_color(sspine_slot slot, sspine_color color);
SOKOL_SPINE_API_DECL sspine_color sspine_slot_get_color(sspine_slot slot);

#ifdef __cplusplus
} // extern "C"
#endif

//-- IMPLEMENTATION ------------------------------------------------------------
#ifdef SOKOL_SPINE_IMPL
#define SOKOL_SPINE_IMPL_INCLUDED (1)

#if !defined(SPINE_SPINE_H_)
#error "Please include spine/spine.h before the sokol_spine.h implementation"
#endif

#include <stdlib.h> // malloc/free
#include <string.h> // memset

#ifndef SOKOL_API_IMPL
    #define SOKOL_API_IMPL
#endif
#ifndef SOKOL_DEBUG
    #ifndef NDEBUG
        #define SOKOL_DEBUG (1)
    #endif
#endif
#ifndef SOKOL_ASSERT
    #include <assert.h>
    #define SOKOL_ASSERT(c) assert(c)
#endif
#ifndef SOKOL_LOG
    #ifdef SOKOL_DEBUG
        #include <stdio.h>
        #define SOKOL_LOG(s) { SOKOL_ASSERT(s); puts(s); }
    #else
        #define SOKOL_LOG(s)
    #endif
#endif
#ifndef SOKOL_UNREACHABLE
    #define SOKOL_UNREACHABLE SOKOL_ASSERT(false)
#endif

/*
    Embedded source compiled with:

    sokol-shdc -i sspine.glsl -o sspine.h -l glsl330:glsl100:hlsl4:metal_macos:metal_ios:metal_sim:wgpu -b

    @vs vs
    uniform vs_params {
        mat4 mvp;
    };
    in vec2 position;
    in vec2 texcoord0;
    in vec4 color0;
    out vec2 uv;
    out vec4 color;
    void main() {
        gl_Position = mvp * vec4(position, 0.0, 1.0);
        uv = texcoord0;
        color = color0;
    }
    @end

    @fs fs
    uniform sampler2D tex;
    uniform fs_params {
        float pma;
    };
    in vec2 uv;
    in vec4 color;
    out vec4 frag_color;
    void main() {
        vec4 c0 = texture(tex, uv);
        vec4 c1 = vec4(c0.rgb * c0.a, c0.a);
        frag_color = mix(c0, c1, pma);
    }
    @end

    @program sspine vs fs
*/
#if defined(SOKOL_GLCORE33)
FIXME
#elif defined(SOKOL_GLES2) || defined(SOKOL_GLES3)
FIXME
#elif defined(SOKOL_D3D11)
FIXME!
#elif defined(SOKOL_METAL)
static const uint8_t _sspine_vs_bytecode_metal_macos[3068] = {
    0x4d,0x54,0x4c,0x42,0x01,0x80,0x02,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xfc,0x0b,0x00,0x00,0x00,0x00,0x00,0x00,0x58,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x6d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc9,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x3b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
    0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
    0xf0,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x6d,0x00,0x00,0x00,
    0x4e,0x41,0x4d,0x45,0x06,0x00,0x6d,0x61,0x69,0x6e,0x30,0x00,0x54,0x59,0x50,0x45,
    0x01,0x00,0x00,0x48,0x41,0x53,0x48,0x20,0x00,0x00,0x1c,0x82,0xf4,0xbb,0x79,0xa7,
    0x30,0x7f,0x4a,0xbc,0xb6,0x93,0x11,0x29,0xf6,0x24,0x44,0x87,0xfa,0x98,0x7e,0xd3,
    0x73,0xca,0x18,0xc6,0xe1,0x22,0x50,0x45,0x09,0x4f,0x46,0x46,0x54,0x18,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0x45,0x52,0x53,0x08,0x00,0x01,0x00,0x08,
    0x00,0x01,0x00,0x01,0x00,0x45,0x4e,0x44,0x54,0x37,0x00,0x00,0x00,0x56,0x41,0x54,
    0x54,0x22,0x00,0x03,0x00,0x70,0x6f,0x73,0x69,0x74,0x69,0x6f,0x6e,0x00,0x00,0x80,
    0x74,0x65,0x78,0x63,0x6f,0x6f,0x72,0x64,0x30,0x00,0x01,0x80,0x63,0x6f,0x6c,0x6f,
    0x72,0x30,0x00,0x02,0x80,0x56,0x41,0x54,0x59,0x05,0x00,0x03,0x00,0x04,0x04,0x06,
    0x45,0x4e,0x44,0x54,0x04,0x00,0x00,0x00,0x45,0x4e,0x44,0x54,0xde,0xc0,0x17,0x0b,
    0x00,0x00,0x00,0x00,0x14,0x00,0x00,0x00,0xdc,0x0a,0x00,0x00,0xff,0xff,0xff,0xff,
    0x42,0x43,0xc0,0xde,0x21,0x0c,0x00,0x00,0xb4,0x02,0x00,0x00,0x0b,0x82,0x20,0x00,
    0x02,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x07,0x81,0x23,0x91,0x41,0xc8,0x04,0x49,
    0x06,0x10,0x32,0x39,0x92,0x01,0x84,0x0c,0x25,0x05,0x08,0x19,0x1e,0x04,0x8b,0x62,
    0x80,0x14,0x45,0x02,0x42,0x92,0x0b,0x42,0xa4,0x10,0x32,0x14,0x38,0x08,0x18,0x49,
    0x0a,0x32,0x44,0x24,0x48,0x0a,0x90,0x21,0x23,0xc4,0x52,0x80,0x0c,0x19,0x21,0x72,
    0x24,0x07,0xc8,0x48,0x11,0x62,0xa8,0xa0,0xa8,0x40,0xc6,0xf0,0x01,0x00,0x00,0x00,
    0x51,0x18,0x00,0x00,0x81,0x00,0x00,0x00,0x1b,0xc8,0x25,0xf8,0xff,0xff,0xff,0xff,
    0x01,0x90,0x80,0x8a,0x18,0x87,0x77,0x90,0x07,0x79,0x28,0x87,0x71,0xa0,0x07,0x76,
    0xc8,0x87,0x36,0x90,0x87,0x77,0xa8,0x07,0x77,0x20,0x87,0x72,0x20,0x87,0x36,0x20,
    0x87,0x74,0xb0,0x87,0x74,0x20,0x87,0x72,0x68,0x83,0x79,0x88,0x07,0x79,0xa0,0x87,
    0x36,0x30,0x07,0x78,0x68,0x83,0x76,0x08,0x07,0x7a,0x40,0x07,0xc0,0x1c,0xc2,0x81,
    0x1d,0xe6,0xa1,0x1c,0x00,0x82,0x1c,0xd2,0x61,0x1e,0xc2,0x41,0x1c,0xd8,0xa1,0x1c,
    0xda,0x80,0x1e,0xc2,0x21,0x1d,0xd8,0xa1,0x0d,0xc6,0x21,0x1c,0xd8,0x81,0x1d,0xe6,
    0x01,0x30,0x87,0x70,0x60,0x87,0x79,0x28,0x07,0x80,0x60,0x87,0x72,0x98,0x87,0x79,
    0x68,0x03,0x78,0x90,0x87,0x72,0x18,0x87,0x74,0x98,0x87,0x72,0x68,0x03,0x73,0x80,
    0x87,0x76,0x08,0x07,0x72,0x00,0xcc,0x21,0x1c,0xd8,0x61,0x1e,0xca,0x01,0x20,0xdc,
    0xe1,0x1d,0xda,0xc0,0x1c,0xe4,0x21,0x1c,0xda,0xa1,0x1c,0xda,0x00,0x1e,0xde,0x21,
    0x1d,0xdc,0x81,0x1e,0xca,0x41,0x1e,0xda,0xa0,0x1c,0xd8,0x21,0x1d,0xda,0x01,0xa0,
    0x07,0x79,0xa8,0x87,0x72,0x00,0x06,0x77,0x78,0x87,0x36,0x30,0x07,0x79,0x08,0x87,
    0x76,0x28,0x87,0x36,0x80,0x87,0x77,0x48,0x07,0x77,0xa0,0x87,0x72,0x90,0x87,0x36,
    0x28,0x07,0x76,0x48,0x87,0x76,0x68,0x03,0x77,0x78,0x07,0x77,0x68,0x03,0x76,0x28,
    0x87,0x70,0x30,0x07,0x80,0x70,0x87,0x77,0x68,0x83,0x74,0x70,0x07,0x73,0x98,0x87,
    0x36,0x30,0x07,0x78,0x68,0x83,0x76,0x08,0x07,0x7a,0x40,0x07,0x80,0x1e,0xe4,0xa1,
    0x1e,0xca,0x01,0x20,0xdc,0xe1,0x1d,0xda,0x40,0x1d,0xea,0xa1,0x1d,0xe0,0xa1,0x0d,
    0xe8,0x21,0x1c,0xc4,0x81,0x1d,0xca,0x61,0x1e,0x00,0x73,0x08,0x07,0x76,0x98,0x87,
    0x72,0x00,0x08,0x77,0x78,0x87,0x36,0x70,0x87,0x70,0x70,0x87,0x79,0x68,0x03,0x73,
    0x80,0x87,0x36,0x68,0x87,0x70,0xa0,0x07,0x74,0x00,0xe8,0x41,0x1e,0xea,0xa1,0x1c,
    0x00,0xc2,0x1d,0xde,0xa1,0x0d,0xe6,0x21,0x1d,0xce,0xc1,0x1d,0xca,0x81,0x1c,0xda,
    0x40,0x1f,0xca,0x41,0x1e,0xde,0x61,0x1e,0xda,0xc0,0x1c,0xe0,0xa1,0x0d,0xda,0x21,
    0x1c,0xe8,0x01,0x1d,0x00,0x7a,0x90,0x87,0x7a,0x28,0x07,0x80,0x70,0x87,0x77,0x68,
    0x03,0x7a,0x90,0x87,0x70,0x80,0x07,0x78,0x48,0x07,0x77,0x38,0x87,0x36,0x68,0x87,
    0x70,0xa0,0x07,0x74,0x00,0xe8,0x41,0x1e,0xea,0xa1,0x1c,0x00,0x62,0x1e,0xe8,0x21,
    0x1c,0xc6,0x61,0x1d,0xda,0x00,0x1e,0xe4,0xe1,0x1d,0xe8,0xa1,0x1c,0xc6,0x81,0x1e,
    0xde,0x41,0x1e,0xda,0x40,0x1c,0xea,0xc1,0x1c,0xcc,0xa1,0x1c,0xe4,0xa1,0x0d,0xe6,
    0x21,0x1d,0xf4,0xa1,0x1c,0x00,0x3c,0x00,0x88,0x7a,0x70,0x87,0x79,0x08,0x07,0x73,
    0x28,0x87,0x36,0x30,0x07,0x78,0x68,0x83,0x76,0x08,0x07,0x7a,0x40,0x07,0x80,0x1e,
    0xe4,0xa1,0x1e,0xca,0x01,0x20,0xea,0x61,0x1e,0xca,0xa1,0x0d,0xe6,0xe1,0x1d,0xcc,
    0x81,0x1e,0xda,0xc0,0x1c,0xd8,0xe1,0x1d,0xc2,0x81,0x1e,0x00,0x73,0x08,0x07,0x76,
    0x98,0x87,0x72,0x00,0x36,0x18,0x02,0x01,0x2c,0x40,0x05,0x00,0x49,0x18,0x00,0x00,
    0x01,0x00,0x00,0x00,0x13,0x84,0x40,0x00,0x89,0x20,0x00,0x00,0x1f,0x00,0x00,0x00,
    0x32,0x22,0x48,0x09,0x20,0x64,0x85,0x04,0x93,0x22,0xa4,0x84,0x04,0x93,0x22,0xe3,
    0x84,0xa1,0x90,0x14,0x12,0x4c,0x8a,0x8c,0x0b,0x84,0xa4,0x4c,0x10,0x44,0x33,0x00,
    0xc3,0x08,0x02,0x30,0x8c,0x40,0x00,0x76,0x08,0x42,0x24,0x81,0x98,0x89,0x9a,0x07,
    0x7a,0x90,0x87,0x7a,0x18,0x07,0x7a,0x70,0x83,0x76,0x28,0x07,0x7a,0x08,0x07,0x76,
    0xd0,0x03,0x3d,0x68,0x87,0x70,0xa0,0x07,0x79,0x48,0x07,0x7c,0x40,0x01,0x39,0x48,
    0x9a,0x22,0x4a,0x98,0xfc,0x4a,0xfa,0x1f,0x20,0x02,0x18,0x09,0x05,0x65,0x10,0xc1,
    0x10,0x4a,0x31,0x42,0x10,0x87,0xd0,0x40,0xc0,0x1c,0x01,0x18,0xa4,0xc0,0x9a,0x23,
    0x00,0x85,0x41,0x04,0x41,0x18,0x46,0x20,0x96,0x11,0x00,0x00,0x13,0xb2,0x70,0x48,
    0x07,0x79,0xb0,0x03,0x3a,0x68,0x83,0x70,0x80,0x07,0x78,0x60,0x87,0x72,0x68,0x83,
    0x76,0x08,0x87,0x71,0x78,0x87,0x79,0xc0,0x87,0x38,0x80,0x03,0x37,0x88,0x83,0x38,
    0x70,0x03,0x38,0xd8,0x70,0x1b,0xe5,0xd0,0x06,0xf0,0xa0,0x07,0x76,0x40,0x07,0x7a,
    0x60,0x07,0x74,0xa0,0x07,0x76,0x40,0x07,0x6d,0x90,0x0e,0x71,0xa0,0x07,0x78,0xa0,
    0x07,0x78,0xd0,0x06,0xe9,0x80,0x07,0x7a,0x80,0x07,0x7a,0x80,0x07,0x6d,0x90,0x0e,
    0x71,0x60,0x07,0x7a,0x10,0x07,0x76,0xa0,0x07,0x71,0x60,0x07,0x6d,0x90,0x0e,0x73,
    0x20,0x07,0x7a,0x30,0x07,0x72,0xa0,0x07,0x73,0x20,0x07,0x6d,0x90,0x0e,0x76,0x40,
    0x07,0x7a,0x60,0x07,0x74,0xa0,0x07,0x76,0x40,0x07,0x6d,0x60,0x0e,0x73,0x20,0x07,
    0x7a,0x30,0x07,0x72,0xa0,0x07,0x73,0x20,0x07,0x6d,0x60,0x0e,0x76,0x40,0x07,0x7a,
    0x60,0x07,0x74,0xa0,0x07,0x76,0x40,0x07,0x6d,0x60,0x0f,0x71,0x60,0x07,0x7a,0x10,
    0x07,0x76,0xa0,0x07,0x71,0x60,0x07,0x6d,0x60,0x0f,0x72,0x40,0x07,0x7a,0x30,0x07,
    0x72,0xa0,0x07,0x73,0x20,0x07,0x6d,0x60,0x0f,0x73,0x20,0x07,0x7a,0x30,0x07,0x72,
    0xa0,0x07,0x73,0x20,0x07,0x6d,0x60,0x0f,0x74,0x80,0x07,0x7a,0x60,0x07,0x74,0xa0,
    0x07,0x76,0x40,0x07,0x6d,0x60,0x0f,0x76,0x40,0x07,0x7a,0x60,0x07,0x74,0xa0,0x07,
    0x76,0x40,0x07,0x6d,0x60,0x0f,0x79,0x60,0x07,0x7a,0x10,0x07,0x72,0x80,0x07,0x7a,
    0x10,0x07,0x72,0x80,0x07,0x6d,0x60,0x0f,0x71,0x20,0x07,0x78,0xa0,0x07,0x71,0x20,
    0x07,0x78,0xa0,0x07,0x71,0x20,0x07,0x78,0xd0,0x06,0xf6,0x10,0x07,0x79,0x20,0x07,
    0x7a,0x20,0x07,0x75,0x60,0x07,0x7a,0x20,0x07,0x75,0x60,0x07,0x6d,0x60,0x0f,0x72,
    0x50,0x07,0x76,0xa0,0x07,0x72,0x50,0x07,0x76,0xa0,0x07,0x72,0x50,0x07,0x76,0xd0,
    0x06,0xf6,0x50,0x07,0x71,0x20,0x07,0x7a,0x50,0x07,0x71,0x20,0x07,0x7a,0x50,0x07,
    0x71,0x20,0x07,0x6d,0x60,0x0f,0x71,0x00,0x07,0x72,0x40,0x07,0x7a,0x10,0x07,0x70,
    0x20,0x07,0x74,0xa0,0x07,0x71,0x00,0x07,0x72,0x40,0x07,0x6d,0xe0,0x0e,0x78,0xa0,
    0x07,0x71,0x60,0x07,0x7a,0x30,0x07,0x72,0x30,0x84,0x49,0x00,0x00,0x08,0x00,0x00,
    0x00,0x00,0x00,0xc8,0x02,0x01,0x00,0x00,0x0a,0x00,0x00,0x00,0x32,0x1e,0x98,0x10,
    0x19,0x11,0x4c,0x90,0x8c,0x09,0x26,0x47,0xc6,0x04,0x43,0x5a,0x25,0x30,0x02,0x50,
    0x04,0x05,0x18,0x50,0x08,0x05,0x51,0x06,0x05,0x42,0x6d,0x04,0x80,0xd8,0x58,0x02,
    0x33,0x00,0x00,0x00,0x79,0x18,0x00,0x00,0xea,0x00,0x00,0x00,0x1a,0x03,0x4c,0x10,
    0x97,0x29,0xa2,0x25,0x10,0xab,0x32,0xb9,0xb9,0xb4,0x37,0xb7,0x21,0xc6,0x32,0x28,
    0x00,0xa3,0x50,0xb9,0x1b,0x43,0x0b,0x93,0xfb,0x9a,0x4b,0xd3,0x2b,0x1b,0x62,0x2c,
    0x81,0x22,0x2c,0x05,0xe3,0x20,0x08,0x0e,0x8e,0xad,0x0c,0xa4,0xad,0x8c,0x2e,0x8c,
    0x0d,0xc4,0xae,0x4c,0x6e,0x2e,0xed,0xcd,0x0d,0x64,0x26,0x06,0x06,0x26,0xc6,0xa5,
    0x46,0x46,0x06,0x04,0xa5,0xad,0x8c,0x2e,0x8c,0xcd,0xac,0xac,0x65,0x26,0x06,0x06,
    0x26,0xc6,0xa5,0x46,0x46,0x26,0x65,0x88,0xa0,0x10,0x43,0x8c,0x25,0x58,0x8c,0x45,
    0x60,0xd1,0x54,0x46,0x17,0xc6,0x36,0x04,0x51,0x8e,0x25,0x58,0x82,0x45,0xe0,0x16,
    0x96,0x26,0xe7,0x32,0xf6,0xd6,0x06,0x97,0xc6,0x56,0xe6,0x42,0x56,0xe6,0xf6,0x26,
    0xd7,0x36,0xf7,0x45,0x96,0x36,0x17,0x26,0xc6,0x56,0x36,0x44,0x50,0x12,0x72,0x61,
    0x69,0x72,0x2e,0x63,0x6f,0x6d,0x70,0x69,0x6c,0x65,0x2e,0x66,0x61,0x73,0x74,0x5f,
    0x6d,0x61,0x74,0x68,0x5f,0x65,0x6e,0x61,0x62,0x6c,0x65,0x43,0x04,0x65,0x61,0x19,
    0x84,0xa5,0xc9,0xb9,0x8c,0xbd,0xb5,0xc1,0xa5,0xb1,0x95,0xb9,0x98,0xc9,0x85,0xb5,
    0x95,0x89,0xd5,0x99,0x99,0x95,0xc9,0x7d,0x99,0x95,0xd1,0x8d,0xa1,0x7d,0x91,0xa5,
    0xcd,0x85,0x89,0xb1,0x95,0x0d,0x11,0x94,0x86,0x51,0x58,0x9a,0x9c,0x8b,0x5d,0x99,
    0x1c,0x5d,0x19,0xde,0xd7,0x5b,0x1d,0x1d,0x5c,0x1d,0x1d,0x97,0xba,0xb9,0x32,0x39,
    0x14,0xb6,0xb7,0x31,0x37,0x98,0x14,0x46,0x61,0x69,0x72,0x2e,0x61,0x72,0x67,0x5f,
    0x74,0x79,0x70,0x65,0x5f,0x6e,0x61,0x6d,0x65,0x34,0xcc,0xd8,0xde,0xc2,0xe8,0x64,
    0xc8,0x84,0xa5,0xc9,0xb9,0x84,0xc9,0x9d,0x7d,0xb9,0x85,0xb5,0x95,0x51,0xa8,0xb3,
    0x1b,0xc2,0x28,0x8f,0x02,0x29,0x91,0x22,0x29,0x93,0x42,0x71,0xa9,0x9b,0x2b,0x93,
    0x43,0x61,0x7b,0x1b,0x73,0x8b,0x49,0xa1,0x61,0xc6,0xf6,0x16,0x46,0x47,0xc3,0x62,
    0xec,0x8d,0xed,0x4d,0x6e,0x08,0xa3,0x3c,0x8a,0xa5,0x44,0xca,0xa5,0x4c,0x0a,0x46,
    0x26,0x2c,0x4d,0xce,0x05,0xee,0x6d,0x2e,0x8d,0x2e,0xed,0xcd,0x8d,0xcb,0x19,0xdb,
    0x17,0xd4,0xdb,0x5c,0x1a,0x5d,0xda,0x9b,0xdb,0x10,0x45,0xd1,0x94,0x48,0xb9,0x94,
    0x49,0xd9,0x86,0x18,0x4a,0xa5,0x64,0x0a,0x47,0x28,0x2c,0x4d,0xce,0xc5,0xae,0x4c,
    0x8e,0xae,0x0c,0xef,0x2b,0xcd,0x0d,0xae,0x8e,0x8e,0x52,0x58,0x9a,0x9c,0x0b,0xdb,
    0xdb,0x58,0x18,0x5d,0xda,0x9b,0xdb,0x57,0x9a,0x1b,0x59,0x19,0x1e,0xbd,0xb3,0x32,
    0xb7,0x32,0xb9,0x30,0xba,0x32,0x32,0x94,0xaf,0xaf,0xb0,0x34,0xb9,0x2f,0x38,0xb6,
    0xb0,0xb1,0x32,0xb4,0x37,0x36,0xb2,0x32,0xb9,0xaf,0xaf,0x14,0x22,0x70,0x6f,0x73,
    0x69,0x74,0x69,0x6f,0x6e,0x43,0xa8,0x45,0x50,0x3c,0xe5,0x5b,0x84,0x25,0x50,0xc0,
    0x40,0x89,0x14,0x49,0x99,0x94,0x30,0x60,0x42,0x57,0x86,0x37,0xf6,0xf6,0x26,0x47,
    0x06,0x33,0x84,0x5a,0x02,0xc5,0x53,0xbe,0x25,0x58,0x02,0x05,0x0c,0x94,0x48,0x91,
    0x94,0x49,0x19,0x03,0x1a,0x63,0x6f,0x6c,0x6f,0x72,0x30,0x43,0xa8,0x65,0x50,0x3c,
    0xe5,0x5b,0x86,0x25,0x50,0xc0,0x40,0x89,0x94,0x4b,0x99,0x94,0x32,0xa0,0x12,0x96,
    0x26,0xe7,0x22,0x56,0x67,0x66,0x56,0x26,0xc7,0x27,0x2c,0x4d,0xce,0x45,0xac,0xce,
    0xcc,0xac,0x4c,0xee,0x6b,0x2e,0x4d,0xaf,0x8c,0x48,0x58,0x9a,0x9c,0x8b,0x5c,0x59,
    0x18,0x19,0xa9,0xb0,0x34,0x39,0x97,0x39,0x3a,0xb9,0xba,0x31,0xba,0x2f,0xba,0x3c,
    0xb8,0xb2,0xaf,0x34,0x37,0xb3,0x37,0x22,0x66,0x6c,0x6f,0x61,0x74,0x34,0x78,0x34,
    0x1c,0xda,0xec,0xe0,0x86,0x28,0x8b,0xb0,0x10,0x8b,0xa0,0xac,0x81,0xc2,0x06,0x8c,
    0xc2,0xd2,0xe4,0x5c,0xc2,0xe4,0xce,0xbe,0xe8,0xf2,0xe0,0xca,0xbe,0xe6,0xd2,0xf4,
    0xca,0x78,0x85,0xa5,0xc9,0xb9,0x84,0xc9,0x9d,0x7d,0xd1,0xe5,0xc1,0x95,0x7d,0x85,
    0xb1,0xa5,0x9d,0xb9,0x7d,0xcd,0xa5,0xe9,0x95,0x31,0xb1,0x9b,0xfb,0x82,0x0b,0x93,
    0x0b,0x6b,0x9b,0xe3,0xf0,0x25,0x13,0x33,0x84,0x0c,0x96,0x43,0x39,0x03,0x05,0x0d,
    0x16,0x42,0xf9,0x16,0x61,0x09,0x94,0x34,0x50,0xd4,0x40,0x69,0x03,0xc5,0x0d,0x16,
    0x42,0x79,0x83,0x05,0x51,0x22,0x05,0x0e,0x94,0x49,0x89,0x83,0x21,0x88,0x22,0x06,
    0x0a,0x19,0x28,0x66,0xa0,0xc8,0xc1,0x10,0x23,0x01,0x94,0x4e,0x99,0x03,0x3e,0x6f,
    0x6d,0x6e,0x69,0x70,0x6f,0x74,0x65,0x6e,0x74,0x20,0x63,0x68,0x61,0x72,0x7c,0xa6,
    0xd2,0xda,0xe0,0xd8,0xca,0x40,0x86,0x56,0x56,0x40,0xa8,0x84,0x82,0x82,0x86,0x08,
    0x8a,0x1d,0x0c,0x31,0x94,0x3a,0x50,0xee,0xa0,0x49,0x86,0x18,0x0a,0x1e,0x28,0x78,
    0xd0,0x24,0x23,0x22,0x76,0x60,0x07,0x7b,0x68,0x07,0x37,0x68,0x87,0x77,0x20,0x87,
    0x7a,0x60,0x87,0x72,0x70,0x03,0x73,0x60,0x87,0x70,0x38,0x87,0x79,0x98,0x22,0x04,
    0xc3,0x08,0x85,0x1d,0xd8,0xc1,0x1e,0xda,0xc1,0x0d,0xd2,0x81,0x1c,0xca,0xc1,0x1d,
    0xe8,0x61,0x4a,0x50,0x8c,0x58,0xc2,0x21,0x1d,0xe4,0xc1,0x0d,0xec,0xa1,0x1c,0xe4,
    0x61,0x1e,0xd2,0xe1,0x1d,0xdc,0x61,0x4a,0x60,0x8c,0xa0,0xc2,0x21,0x1d,0xe4,0xc1,
    0x0d,0xd8,0x21,0x1c,0xdc,0xe1,0x1c,0xea,0x21,0x1c,0xce,0xa1,0x1c,0x7e,0xc1,0x1e,
    0xca,0x41,0x1e,0xe6,0x21,0x1d,0xde,0xc1,0x1d,0xa6,0x04,0xc8,0x88,0x29,0x1c,0xd2,
    0x41,0x1e,0xdc,0x60,0x1c,0xde,0xa1,0x1d,0xe0,0x21,0x1d,0xd8,0xa1,0x1c,0x7e,0xe1,
    0x1d,0xe0,0x81,0x1e,0xd2,0xe1,0x1d,0xdc,0x61,0x1e,0xa6,0x0c,0x0a,0xe3,0x8c,0x50,
    0xc2,0x21,0x1d,0xe4,0xc1,0x0d,0xec,0xa1,0x1c,0xe4,0x81,0x1e,0xca,0x01,0x1f,0xa6,
    0x04,0x74,0x00,0x00,0x79,0x18,0x00,0x00,0x7b,0x00,0x00,0x00,0x33,0x08,0x80,0x1c,
    0xc4,0xe1,0x1c,0x66,0x14,0x01,0x3d,0x88,0x43,0x38,0x84,0xc3,0x8c,0x42,0x80,0x07,
    0x79,0x78,0x07,0x73,0x98,0x71,0x0c,0xe6,0x00,0x0f,0xed,0x10,0x0e,0xf4,0x80,0x0e,
    0x33,0x0c,0x42,0x1e,0xc2,0xc1,0x1d,0xce,0xa1,0x1c,0x66,0x30,0x05,0x3d,0x88,0x43,
    0x38,0x84,0x83,0x1b,0xcc,0x03,0x3d,0xc8,0x43,0x3d,0x8c,0x03,0x3d,0xcc,0x78,0x8c,
    0x74,0x70,0x07,0x7b,0x08,0x07,0x79,0x48,0x87,0x70,0x70,0x07,0x7a,0x70,0x03,0x76,
    0x78,0x87,0x70,0x20,0x87,0x19,0xcc,0x11,0x0e,0xec,0x90,0x0e,0xe1,0x30,0x0f,0x6e,
    0x30,0x0f,0xe3,0xf0,0x0e,0xf0,0x50,0x0e,0x33,0x10,0xc4,0x1d,0xde,0x21,0x1c,0xd8,
    0x21,0x1d,0xc2,0x61,0x1e,0x66,0x30,0x89,0x3b,0xbc,0x83,0x3b,0xd0,0x43,0x39,0xb4,
    0x03,0x3c,0xbc,0x83,0x3c,0x84,0x03,0x3b,0xcc,0xf0,0x14,0x76,0x60,0x07,0x7b,0x68,
    0x07,0x37,0x68,0x87,0x72,0x68,0x07,0x37,0x80,0x87,0x70,0x90,0x87,0x70,0x60,0x07,
    0x76,0x28,0x07,0x76,0xf8,0x05,0x76,0x78,0x87,0x77,0x80,0x87,0x5f,0x08,0x87,0x71,
    0x18,0x87,0x72,0x98,0x87,0x79,0x98,0x81,0x2c,0xee,0xf0,0x0e,0xee,0xe0,0x0e,0xf5,
    0xc0,0x0e,0xec,0x30,0x03,0x62,0xc8,0xa1,0x1c,0xe4,0xa1,0x1c,0xcc,0xa1,0x1c,0xe4,
    0xa1,0x1c,0xdc,0x61,0x1c,0xca,0x21,0x1c,0xc4,0x81,0x1d,0xca,0x61,0x06,0xd6,0x90,
    0x43,0x39,0xc8,0x43,0x39,0x98,0x43,0x39,0xc8,0x43,0x39,0xb8,0xc3,0x38,0x94,0x43,
    0x38,0x88,0x03,0x3b,0x94,0xc3,0x2f,0xbc,0x83,0x3c,0xfc,0x82,0x3b,0xd4,0x03,0x3b,
    0xb0,0xc3,0x0c,0xc7,0x69,0x87,0x70,0x58,0x87,0x72,0x70,0x83,0x74,0x68,0x07,0x78,
    0x60,0x87,0x74,0x18,0x87,0x74,0xa0,0x87,0x19,0xce,0x53,0x0f,0xee,0x00,0x0f,0xf2,
    0x50,0x0e,0xe4,0x90,0x0e,0xe3,0x40,0x0f,0xe1,0x20,0x0e,0xec,0x50,0x0e,0x33,0x20,
    0x28,0x1d,0xdc,0xc1,0x1e,0xc2,0x41,0x1e,0xd2,0x21,0x1c,0xdc,0x81,0x1e,0xdc,0xe0,
    0x1c,0xe4,0xe1,0x1d,0xea,0x01,0x1e,0x66,0x18,0x51,0x38,0xb0,0x43,0x3a,0x9c,0x83,
    0x3b,0xcc,0x50,0x24,0x76,0x60,0x07,0x7b,0x68,0x07,0x37,0x60,0x87,0x77,0x78,0x07,
    0x78,0x98,0x51,0x4c,0xf4,0x90,0x0f,0xf0,0x50,0x0e,0x33,0x1e,0x6a,0x1e,0xca,0x61,
    0x1c,0xe8,0x21,0x1d,0xde,0xc1,0x1d,0x7e,0x01,0x1e,0xe4,0xa1,0x1c,0xcc,0x21,0x1d,
    0xf0,0x61,0x06,0x54,0x85,0x83,0x38,0xcc,0xc3,0x3b,0xb0,0x43,0x3d,0xd0,0x43,0x39,
    0xfc,0xc2,0x3c,0xe4,0x43,0x3b,0x88,0xc3,0x3b,0xb0,0xc3,0x8c,0xc5,0x0a,0x87,0x79,
    0x98,0x87,0x77,0x18,0x87,0x74,0x08,0x07,0x7a,0x28,0x07,0x72,0x98,0x81,0x5c,0xe3,
    0x10,0x0e,0xec,0xc0,0x0e,0xe5,0x50,0x0e,0xf3,0x30,0x23,0xc1,0xd2,0x41,0x1e,0xe4,
    0xe1,0x17,0xd8,0xe1,0x1d,0xde,0x01,0x1e,0x66,0x50,0x59,0x38,0xa4,0x83,0x3c,0xb8,
    0x81,0x39,0xd4,0x83,0x3b,0x8c,0x03,0x3d,0xa4,0xc3,0x3b,0xb8,0xc3,0x2f,0x9c,0x83,
    0x3c,0xbc,0x43,0x3d,0xc0,0xc3,0x3c,0x00,0x71,0x20,0x00,0x00,0x02,0x00,0x00,0x00,
    0x06,0x50,0x30,0x00,0xd2,0xd0,0x00,0x00,0x61,0x20,0x00,0x00,0x1e,0x00,0x00,0x00,
    0x13,0x04,0x41,0x2c,0x10,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0xf4,0xc6,0x22,0x82,
    0x20,0x08,0x46,0x00,0xa8,0x95,0x40,0x19,0xd0,0x98,0x01,0xa0,0x30,0x03,0x00,0x00,
    0xe3,0x15,0x07,0x33,0x4d,0x0c,0x05,0x65,0x90,0x81,0x19,0x0e,0x13,0x02,0xf9,0x8c,
    0x57,0x2c,0xd0,0x75,0x21,0x14,0x94,0x41,0x06,0xe8,0x60,0x4c,0x08,0xe4,0x63,0x41,
    0x01,0x9f,0xf1,0x0a,0xa8,0xe2,0x38,0x86,0x82,0x62,0x43,0x00,0x9f,0xd9,0x06,0xa7,
    0x02,0x66,0x1b,0x82,0x2a,0x98,0x6d,0x08,0x06,0x21,0x83,0x80,0x18,0x00,0x00,0x00,
    0x04,0x00,0x00,0x00,0x5b,0x86,0x20,0xc8,0x83,0x2d,0x43,0x11,0xe4,0xc1,0x96,0x41,
    0x09,0xf2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
static const uint8_t _sspine_fs_bytecode_metal_macos[3209] = {
    0x4d,0x54,0x4c,0x42,0x01,0x80,0x02,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x89,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x58,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x6d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc9,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xd1,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xd9,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xb0,0x0b,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x6d,0x00,0x00,0x00,
    0x4e,0x41,0x4d,0x45,0x06,0x00,0x6d,0x61,0x69,0x6e,0x30,0x00,0x54,0x59,0x50,0x45,
    0x01,0x00,0x01,0x48,0x41,0x53,0x48,0x20,0x00,0x20,0xca,0xe9,0xff,0xb6,0x0d,0x6c,
    0xae,0x70,0x37,0x1b,0x1b,0x2a,0x69,0xd8,0x5a,0x90,0x7d,0x57,0xb3,0xb9,0xd2,0xed,
    0x8f,0xc1,0x3d,0x5d,0x11,0x9f,0x6d,0x28,0xc3,0x4f,0x46,0x46,0x54,0x18,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0x45,0x52,0x53,0x08,0x00,0x01,0x00,0x08,
    0x00,0x01,0x00,0x01,0x00,0x45,0x4e,0x44,0x54,0x04,0x00,0x00,0x00,0x45,0x4e,0x44,
    0x54,0x04,0x00,0x00,0x00,0x45,0x4e,0x44,0x54,0xde,0xc0,0x17,0x0b,0x00,0x00,0x00,
    0x00,0x14,0x00,0x00,0x00,0x98,0x0b,0x00,0x00,0xff,0xff,0xff,0xff,0x42,0x43,0xc0,
    0xde,0x21,0x0c,0x00,0x00,0xe3,0x02,0x00,0x00,0x0b,0x82,0x20,0x00,0x02,0x00,0x00,
    0x00,0x12,0x00,0x00,0x00,0x07,0x81,0x23,0x91,0x41,0xc8,0x04,0x49,0x06,0x10,0x32,
    0x39,0x92,0x01,0x84,0x0c,0x25,0x05,0x08,0x19,0x1e,0x04,0x8b,0x62,0x80,0x14,0x45,
    0x02,0x42,0x92,0x0b,0x42,0xa4,0x10,0x32,0x14,0x38,0x08,0x18,0x49,0x0a,0x32,0x44,
    0x24,0x48,0x0a,0x90,0x21,0x23,0xc4,0x52,0x80,0x0c,0x19,0x21,0x72,0x24,0x07,0xc8,
    0x48,0x11,0x62,0xa8,0xa0,0xa8,0x40,0xc6,0xf0,0x01,0x00,0x00,0x00,0x51,0x18,0x00,
    0x00,0x8e,0x00,0x00,0x00,0x1b,0xcc,0x25,0xf8,0xff,0xff,0xff,0xff,0x01,0x60,0x00,
    0x09,0xa8,0x88,0x71,0x78,0x07,0x79,0x90,0x87,0x72,0x18,0x07,0x7a,0x60,0x87,0x7c,
    0x68,0x03,0x79,0x78,0x87,0x7a,0x70,0x07,0x72,0x28,0x07,0x72,0x68,0x03,0x72,0x48,
    0x07,0x7b,0x48,0x07,0x72,0x28,0x87,0x36,0x98,0x87,0x78,0x90,0x07,0x7a,0x68,0x03,
    0x73,0x80,0x87,0x36,0x68,0x87,0x70,0xa0,0x07,0x74,0x00,0xcc,0x21,0x1c,0xd8,0x61,
    0x1e,0xca,0x01,0x20,0xc8,0x21,0x1d,0xe6,0x21,0x1c,0xc4,0x81,0x1d,0xca,0xa1,0x0d,
    0xe8,0x21,0x1c,0xd2,0x81,0x1d,0xda,0x60,0x1c,0xc2,0x81,0x1d,0xd8,0x61,0x1e,0x00,
    0x73,0x08,0x07,0x76,0x98,0x87,0x72,0x00,0x08,0x76,0x28,0x87,0x79,0x98,0x87,0x36,
    0x80,0x07,0x79,0x28,0x87,0x71,0x48,0x87,0x79,0x28,0x87,0x36,0x30,0x07,0x78,0x68,
    0x87,0x70,0x20,0x07,0xc0,0x1c,0xc2,0x81,0x1d,0xe6,0xa1,0x1c,0x00,0xc2,0x1d,0xde,
    0xa1,0x0d,0xcc,0x41,0x1e,0xc2,0xa1,0x1d,0xca,0xa1,0x0d,0xe0,0xe1,0x1d,0xd2,0xc1,
    0x1d,0xe8,0xa1,0x1c,0xe4,0xa1,0x0d,0xca,0x81,0x1d,0xd2,0xa1,0x1d,0x00,0x7a,0x90,
    0x87,0x7a,0x28,0x07,0x60,0x70,0x87,0x77,0x68,0x03,0x73,0x90,0x87,0x70,0x68,0x87,
    0x72,0x68,0x03,0x78,0x78,0x87,0x74,0x70,0x07,0x7a,0x28,0x07,0x79,0x68,0x83,0x72,
    0x60,0x87,0x74,0x68,0x87,0x36,0x70,0x87,0x77,0x70,0x87,0x36,0x60,0x87,0x72,0x08,
    0x07,0x73,0x00,0x08,0x77,0x78,0x87,0x36,0x48,0x07,0x77,0x30,0x87,0x79,0x68,0x03,
    0x73,0x80,0x87,0x36,0x68,0x87,0x70,0xa0,0x07,0x74,0x00,0xe8,0x41,0x1e,0xea,0xa1,
    0x1c,0x00,0xc2,0x1d,0xde,0xa1,0x0d,0xd4,0xa1,0x1e,0xda,0x01,0x1e,0xda,0x80,0x1e,
    0xc2,0x41,0x1c,0xd8,0xa1,0x1c,0xe6,0x01,0x30,0x87,0x70,0x60,0x87,0x79,0x28,0x07,
    0x80,0x70,0x87,0x77,0x68,0x03,0x77,0x08,0x07,0x77,0x98,0x87,0x36,0x30,0x07,0x78,
    0x68,0x83,0x76,0x08,0x07,0x7a,0x40,0x07,0x80,0x1e,0xe4,0xa1,0x1e,0xca,0x01,0x20,
    0xdc,0xe1,0x1d,0xda,0x60,0x1e,0xd2,0xe1,0x1c,0xdc,0xa1,0x1c,0xc8,0xa1,0x0d,0xf4,
    0xa1,0x1c,0xe4,0xe1,0x1d,0xe6,0xa1,0x0d,0xcc,0x01,0x1e,0xda,0xa0,0x1d,0xc2,0x81,
    0x1e,0xd0,0x01,0xa0,0x07,0x79,0xa8,0x87,0x72,0x00,0x08,0x77,0x78,0x87,0x36,0xa0,
    0x07,0x79,0x08,0x07,0x78,0x80,0x87,0x74,0x70,0x87,0x73,0x68,0x83,0x76,0x08,0x07,
    0x7a,0x40,0x07,0x80,0x1e,0xe4,0xa1,0x1e,0xca,0x01,0x20,0xe6,0x81,0x1e,0xc2,0x61,
    0x1c,0xd6,0xa1,0x0d,0xe0,0x41,0x1e,0xde,0x81,0x1e,0xca,0x61,0x1c,0xe8,0xe1,0x1d,
    0xe4,0xa1,0x0d,0xc4,0xa1,0x1e,0xcc,0xc1,0x1c,0xca,0x41,0x1e,0xda,0x60,0x1e,0xd2,
    0x41,0x1f,0xca,0x01,0xc0,0x03,0x80,0xa8,0x07,0x77,0x98,0x87,0x70,0x30,0x87,0x72,
    0x68,0x03,0x73,0x80,0x87,0x36,0x68,0x87,0x70,0xa0,0x07,0x74,0x00,0xe8,0x41,0x1e,
    0xea,0xa1,0x1c,0x00,0xa2,0x1e,0xe6,0xa1,0x1c,0xda,0x60,0x1e,0xde,0xc1,0x1c,0xe8,
    0xa1,0x0d,0xcc,0x81,0x1d,0xde,0x21,0x1c,0xe8,0x01,0x30,0x87,0x70,0x60,0x87,0x79,
    0x28,0x07,0x60,0x83,0x21,0x08,0xc0,0x02,0x54,0x1b,0x8c,0x61,0x00,0x16,0xa0,0xda,
    0x60,0x10,0x04,0xb0,0x00,0xd5,0x06,0xa3,0xf8,0xff,0xff,0xff,0xff,0x01,0x90,0x00,
    0x6a,0x03,0x62,0xfc,0xff,0xff,0xff,0xff,0x00,0x30,0x80,0x04,0x54,0x1b,0x8c,0x23,
    0x00,0x16,0xa0,0xda,0x60,0x20,0x02,0xb0,0x00,0x15,0x00,0x00,0x00,0x49,0x18,0x00,
    0x00,0x03,0x00,0x00,0x00,0x13,0x88,0x40,0x18,0x88,0x09,0x41,0x31,0x61,0x30,0x0e,
    0x04,0x89,0x20,0x00,0x00,0x27,0x00,0x00,0x00,0x32,0x22,0x48,0x09,0x20,0x64,0x85,
    0x04,0x93,0x22,0xa4,0x84,0x04,0x93,0x22,0xe3,0x84,0xa1,0x90,0x14,0x12,0x4c,0x8a,
    0x8c,0x0b,0x84,0xa4,0x4c,0x10,0x6c,0x33,0x00,0xc3,0x08,0x04,0x60,0x83,0x30,0x8c,
    0x20,0x00,0x07,0x49,0x53,0x44,0x09,0x93,0x5f,0x48,0xff,0x03,0x44,0x00,0x23,0xa1,
    0x00,0x0c,0x22,0x10,0xc2,0x51,0xd2,0x14,0x51,0xc2,0xe4,0xff,0x13,0x71,0x4d,0x54,
    0x44,0xfc,0xf6,0xf0,0x4f,0x63,0x04,0xc0,0x20,0x82,0x11,0x5c,0x24,0x4d,0x11,0x25,
    0x4c,0xfe,0x2f,0x01,0xcc,0xb3,0x10,0xd1,0x3f,0x8d,0x11,0x00,0x83,0x08,0x88,0x50,
    0x0a,0x31,0xca,0x49,0xa4,0x0a,0x11,0x42,0x08,0xc4,0xe6,0x08,0x82,0x39,0x02,0x30,
    0x18,0x46,0x10,0x9e,0x82,0x84,0x93,0x86,0x83,0x0e,0x50,0x1c,0x08,0x48,0x81,0x37,
    0x8c,0x30,0x3c,0xc3,0x08,0x03,0x30,0x8c,0x40,0x3c,0x73,0x04,0xa0,0x30,0x88,0x00,
    0x08,0x23,0x00,0x00,0x00,0x13,0xb2,0x70,0x48,0x07,0x79,0xb0,0x03,0x3a,0x68,0x83,
    0x70,0x80,0x07,0x78,0x60,0x87,0x72,0x68,0x83,0x76,0x08,0x87,0x71,0x78,0x87,0x79,
    0xc0,0x87,0x38,0x80,0x03,0x37,0x88,0x83,0x38,0x70,0x03,0x38,0xd8,0x70,0x1b,0xe5,
    0xd0,0x06,0xf0,0xa0,0x07,0x76,0x40,0x07,0x7a,0x60,0x07,0x74,0xa0,0x07,0x76,0x40,
    0x07,0x6d,0x90,0x0e,0x71,0xa0,0x07,0x78,0xa0,0x07,0x78,0xd0,0x06,0xe9,0x80,0x07,
    0x7a,0x80,0x07,0x7a,0x80,0x07,0x6d,0x90,0x0e,0x71,0x60,0x07,0x7a,0x10,0x07,0x76,
    0xa0,0x07,0x71,0x60,0x07,0x6d,0x90,0x0e,0x73,0x20,0x07,0x7a,0x30,0x07,0x72,0xa0,
    0x07,0x73,0x20,0x07,0x6d,0x90,0x0e,0x76,0x40,0x07,0x7a,0x60,0x07,0x74,0xa0,0x07,
    0x76,0x40,0x07,0x6d,0x60,0x0e,0x73,0x20,0x07,0x7a,0x30,0x07,0x72,0xa0,0x07,0x73,
    0x20,0x07,0x6d,0x60,0x0e,0x76,0x40,0x07,0x7a,0x60,0x07,0x74,0xa0,0x07,0x76,0x40,
    0x07,0x6d,0x60,0x0f,0x71,0x60,0x07,0x7a,0x10,0x07,0x76,0xa0,0x07,0x71,0x60,0x07,
    0x6d,0x60,0x0f,0x72,0x40,0x07,0x7a,0x30,0x07,0x72,0xa0,0x07,0x73,0x20,0x07,0x6d,
    0x60,0x0f,0x73,0x20,0x07,0x7a,0x30,0x07,0x72,0xa0,0x07,0x73,0x20,0x07,0x6d,0x60,
    0x0f,0x74,0x80,0x07,0x7a,0x60,0x07,0x74,0xa0,0x07,0x76,0x40,0x07,0x6d,0x60,0x0f,
    0x76,0x40,0x07,0x7a,0x60,0x07,0x74,0xa0,0x07,0x76,0x40,0x07,0x6d,0x60,0x0f,0x79,
    0x60,0x07,0x7a,0x10,0x07,0x72,0x80,0x07,0x7a,0x10,0x07,0x72,0x80,0x07,0x6d,0x60,
    0x0f,0x71,0x20,0x07,0x78,0xa0,0x07,0x71,0x20,0x07,0x78,0xa0,0x07,0x71,0x20,0x07,
    0x78,0xd0,0x06,0xf6,0x10,0x07,0x79,0x20,0x07,0x7a,0x20,0x07,0x75,0x60,0x07,0x7a,
    0x20,0x07,0x75,0x60,0x07,0x6d,0x60,0x0f,0x72,0x50,0x07,0x76,0xa0,0x07,0x72,0x50,
    0x07,0x76,0xa0,0x07,0x72,0x50,0x07,0x76,0xd0,0x06,0xf6,0x50,0x07,0x71,0x20,0x07,
    0x7a,0x50,0x07,0x71,0x20,0x07,0x7a,0x50,0x07,0x71,0x20,0x07,0x6d,0x60,0x0f,0x71,
    0x00,0x07,0x72,0x40,0x07,0x7a,0x10,0x07,0x70,0x20,0x07,0x74,0xa0,0x07,0x71,0x00,
    0x07,0x72,0x40,0x07,0x6d,0xe0,0x0e,0x78,0xa0,0x07,0x71,0x60,0x07,0x7a,0x30,0x07,
    0x72,0x30,0x84,0x59,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x18,0xc2,0x34,0x40,
    0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x0c,0x61,0x24,0x20,0x00,0x06,0x00,0x00,0x00,
    0x00,0x00,0xb2,0x40,0x00,0x09,0x00,0x00,0x00,0x32,0x1e,0x98,0x10,0x19,0x11,0x4c,
    0x90,0x8c,0x09,0x26,0x47,0xc6,0x04,0x43,0x7a,0x23,0x00,0x25,0x50,0x08,0x45,0x50,
    0x10,0x65,0x40,0x78,0x04,0x80,0xe8,0x58,0x02,0x33,0x00,0x00,0x00,0x79,0x18,0x00,
    0x00,0xf0,0x00,0x00,0x00,0x1a,0x03,0x4c,0x10,0x97,0x29,0xa2,0x25,0x10,0xab,0x32,
    0xb9,0xb9,0xb4,0x37,0xb7,0x21,0xc6,0x63,0x4c,0x00,0xa5,0x50,0xb9,0x1b,0x43,0x0b,
    0x93,0xfb,0x9a,0x4b,0xd3,0x2b,0x1b,0x62,0x3c,0xc4,0x24,0x3c,0x05,0xe3,0x20,0x08,
    0x0e,0x8e,0xad,0x0c,0xa4,0xad,0x8c,0x2e,0x8c,0x0d,0xc4,0xae,0x4c,0x6e,0x2e,0xed,
    0xcd,0x0d,0x64,0x26,0x06,0x06,0x26,0xc6,0xa5,0x46,0x46,0x06,0x04,0xa5,0xad,0x8c,
    0x2e,0x8c,0xcd,0xac,0xac,0x65,0x26,0x06,0x06,0x26,0xc6,0xa5,0x46,0x46,0x26,0x65,
    0x88,0x30,0x11,0x43,0x8c,0x87,0x78,0x8e,0x67,0x60,0xd1,0x54,0x46,0x17,0xc6,0x36,
    0x04,0x99,0x8e,0x87,0x78,0x88,0x67,0xe0,0x16,0x96,0x26,0xe7,0x32,0xf6,0xd6,0x06,
    0x97,0xc6,0x56,0xe6,0x42,0x56,0xe6,0xf6,0x26,0xd7,0x36,0xf7,0x45,0x96,0x36,0x17,
    0x26,0xc6,0x56,0x36,0x44,0x98,0x12,0x72,0x61,0x69,0x72,0x2e,0x63,0x6f,0x6d,0x70,
    0x69,0x6c,0x65,0x2e,0x66,0x61,0x73,0x74,0x5f,0x6d,0x61,0x74,0x68,0x5f,0x65,0x6e,
    0x61,0x62,0x6c,0x65,0x43,0x84,0x69,0x61,0x19,0x84,0xa5,0xc9,0xb9,0x8c,0xbd,0xb5,
    0xc1,0xa5,0xb1,0x95,0xb9,0x98,0xc9,0x85,0xb5,0x95,0x89,0xd5,0x99,0x99,0x95,0xc9,
    0x7d,0x99,0x95,0xd1,0x8d,0xa1,0x7d,0x91,0xa5,0xcd,0x85,0x89,0xb1,0x95,0x0d,0x11,
    0xa6,0x86,0x51,0x58,0x9a,0x9c,0x8b,0x5c,0x99,0x1b,0x59,0x99,0xdc,0x17,0x5d,0x98,
    0xdc,0x59,0x19,0x1d,0xa3,0xb0,0x34,0x39,0x97,0x30,0xb9,0xb3,0x2f,0xba,0x3c,0xb8,
    0xb2,0x2f,0xb7,0xb0,0xb6,0x32,0x1a,0x66,0x6c,0x6f,0x61,0x74,0x34,0x64,0xc2,0xd2,
    0xe4,0x5c,0xc2,0xe4,0xce,0xbe,0xdc,0xc2,0xda,0xca,0xa8,0x98,0xc9,0x85,0x9d,0x7d,
    0x8d,0xbd,0xb1,0xbd,0xc9,0x0d,0x61,0xa6,0xe7,0x19,0x26,0x68,0x8a,0x26,0x69,0x9a,
    0x86,0x08,0x13,0x45,0x29,0x2c,0x4d,0xce,0xc5,0x4c,0x2e,0xec,0xac,0xad,0xcc,0x8d,
    0xee,0x2b,0xcd,0x0d,0xae,0x8e,0x8e,0x4b,0xdd,0x5c,0x99,0x1c,0x0a,0xdb,0xdb,0x98,
    0x1b,0x4c,0x0a,0x95,0xb0,0x34,0x39,0x97,0xb1,0x32,0x37,0xba,0x32,0x39,0x3e,0x61,
    0x69,0x72,0x2e,0x70,0x65,0x72,0x73,0x70,0x65,0x63,0x74,0x69,0x76,0x65,0x34,0xcc,
    0xd8,0xde,0xc2,0xe8,0x64,0x28,0xd4,0xd9,0x0d,0x91,0x9e,0x61,0xb2,0xa6,0x6b,0xc2,
    0xa6,0x6c,0x82,0x26,0x6d,0x92,0xa6,0x8d,0x4a,0x58,0x9a,0x9c,0x8b,0x58,0x9d,0x99,
    0x59,0x99,0x1c,0x9f,0xb0,0x34,0x39,0x17,0xb1,0x3a,0x33,0xb3,0x32,0xb9,0xaf,0xb9,
    0x34,0xbd,0x32,0x4a,0x61,0x69,0x72,0x2e,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,
    0x5f,0x69,0x6e,0x64,0x65,0x78,0x44,0xc2,0xd2,0xe4,0x5c,0xe4,0xca,0xc2,0xc8,0x48,
    0x85,0xa5,0xc9,0xb9,0xcc,0xd1,0xc9,0xd5,0x8d,0xd1,0x7d,0xd1,0xe5,0xc1,0x95,0x7d,
    0xa5,0xb9,0x99,0xbd,0xb1,0x30,0x63,0x7b,0x0b,0xa3,0xe3,0x00,0xd7,0x16,0x36,0x44,
    0x79,0x86,0xa7,0x78,0x86,0x49,0x0c,0xa6,0x31,0x60,0x14,0x96,0x26,0xe7,0x12,0x26,
    0x77,0xf6,0x45,0x97,0x07,0x57,0xf6,0x35,0x97,0xa6,0x57,0xc6,0x2b,0x2c,0x4d,0xce,
    0x25,0x4c,0xee,0xec,0x8b,0x2e,0x0f,0xae,0xec,0x2b,0x8c,0x2d,0xed,0xcc,0xed,0x6b,
    0x2e,0x4d,0xaf,0x8c,0x89,0xd9,0xdc,0x17,0x5c,0x98,0x5c,0x58,0xdb,0x1c,0x87,0x2f,
    0x1a,0x9a,0x21,0x64,0xf0,0x10,0x53,0x37,0x79,0x4f,0x31,0x7d,0xcf,0xf0,0x10,0x13,
    0x18,0x4c,0x61,0x30,0x91,0xc1,0x54,0x06,0x4f,0x31,0x99,0xc1,0x53,0x4c,0xd0,0x74,
    0x06,0x93,0x34,0xa1,0x01,0x97,0xb0,0x34,0x39,0x17,0xba,0x32,0x3c,0xba,0x3a,0xb9,
    0x32,0x2a,0x61,0x69,0x72,0x2e,0x73,0x61,0x6d,0x70,0x6c,0x65,0xc4,0xe8,0xca,0xf0,
    0xe8,0xea,0xe4,0xca,0x64,0xc8,0x78,0xcc,0xd8,0xde,0xc2,0xe8,0x58,0x40,0xe6,0xc2,
    0xda,0xe0,0xd8,0xca,0x7c,0x38,0xd0,0x95,0xe1,0x0d,0xa1,0x1e,0x63,0x52,0x83,0xe9,
    0x7b,0x86,0x87,0x98,0xd6,0x60,0x82,0x26,0x36,0x98,0xa4,0xa9,0x0d,0xb8,0x84,0xa5,
    0xc9,0xb9,0xcc,0x85,0xb5,0xc1,0xb1,0x95,0xc9,0xf1,0x98,0x0b,0x6b,0x83,0x63,0x2b,
    0x93,0x23,0x42,0x57,0x86,0x37,0xd5,0x06,0xc7,0x26,0x37,0x44,0x7a,0x90,0xe9,0x0d,
    0xa6,0xef,0x19,0x1e,0x62,0x82,0x26,0x38,0x98,0xa4,0x29,0x0e,0x86,0x20,0x13,0x37,
    0xa5,0xc1,0xe4,0x06,0x93,0x1c,0x0c,0x31,0x16,0x60,0xaa,0xa6,0x39,0xa0,0xf3,0xa5,
    0x45,0x35,0x95,0x63,0x36,0xf7,0x05,0x17,0x26,0x17,0xd6,0x36,0xc7,0xe7,0xad,0xcd,
    0x2d,0x0d,0xee,0x8d,0xae,0xcc,0x8d,0x0e,0x64,0x0c,0x2d,0x4c,0x8e,0xcf,0x54,0x5a,
    0x1b,0x1c,0x5b,0x19,0xc8,0xd0,0xca,0x0a,0x08,0x95,0x50,0x50,0xd0,0x10,0x61,0xba,
    0x83,0x21,0xc6,0x64,0x07,0x13,0x1e,0x60,0xc9,0x10,0x63,0x12,0x83,0x29,0x0f,0xb0,
    0x64,0x88,0x31,0xd5,0xc1,0xa4,0x07,0x58,0x32,0xc4,0x98,0xf6,0x60,0xd2,0x03,0x2c,
    0x19,0x11,0xb1,0x03,0x3b,0xd8,0x43,0x3b,0xb8,0x41,0x3b,0xbc,0x03,0x39,0xd4,0x03,
    0x3b,0x94,0x83,0x1b,0x98,0x03,0x3b,0x84,0xc3,0x39,0xcc,0xc3,0x14,0x21,0x18,0x46,
    0x28,0xec,0xc0,0x0e,0xf6,0xd0,0x0e,0x6e,0x90,0x0e,0xe4,0x50,0x0e,0xee,0x40,0x0f,
    0x53,0x82,0x62,0xc4,0x12,0x0e,0xe9,0x20,0x0f,0x6e,0x60,0x0f,0xe5,0x20,0x0f,0xf3,
    0x90,0x0e,0xef,0xe0,0x0e,0x53,0x02,0x63,0x04,0x15,0x0e,0xe9,0x20,0x0f,0x6e,0xc0,
    0x0e,0xe1,0xe0,0x0e,0xe7,0x50,0x0f,0xe1,0x70,0x0e,0xe5,0xf0,0x0b,0xf6,0x50,0x0e,
    0xf2,0x30,0x0f,0xe9,0xf0,0x0e,0xee,0x30,0x25,0x40,0x46,0x4c,0xe1,0x90,0x0e,0xf2,
    0xe0,0x06,0xe3,0xf0,0x0e,0xed,0x00,0x0f,0xe9,0xc0,0x0e,0xe5,0xf0,0x0b,0xef,0x00,
    0x0f,0xf4,0x90,0x0e,0xef,0xe0,0x0e,0xf3,0x30,0x65,0x50,0x18,0x67,0x04,0x13,0x0e,
    0xe9,0x20,0x0f,0x6e,0x60,0x0e,0xf2,0x10,0x0e,0xe7,0xd0,0x0e,0xe5,0xe0,0x0e,0xf4,
    0x30,0x25,0xa0,0x03,0x00,0x79,0x18,0x00,0x00,0x7b,0x00,0x00,0x00,0x33,0x08,0x80,
    0x1c,0xc4,0xe1,0x1c,0x66,0x14,0x01,0x3d,0x88,0x43,0x38,0x84,0xc3,0x8c,0x42,0x80,
    0x07,0x79,0x78,0x07,0x73,0x98,0x71,0x0c,0xe6,0x00,0x0f,0xed,0x10,0x0e,0xf4,0x80,
    0x0e,0x33,0x0c,0x42,0x1e,0xc2,0xc1,0x1d,0xce,0xa1,0x1c,0x66,0x30,0x05,0x3d,0x88,
    0x43,0x38,0x84,0x83,0x1b,0xcc,0x03,0x3d,0xc8,0x43,0x3d,0x8c,0x03,0x3d,0xcc,0x78,
    0x8c,0x74,0x70,0x07,0x7b,0x08,0x07,0x79,0x48,0x87,0x70,0x70,0x07,0x7a,0x70,0x03,
    0x76,0x78,0x87,0x70,0x20,0x87,0x19,0xcc,0x11,0x0e,0xec,0x90,0x0e,0xe1,0x30,0x0f,
    0x6e,0x30,0x0f,0xe3,0xf0,0x0e,0xf0,0x50,0x0e,0x33,0x10,0xc4,0x1d,0xde,0x21,0x1c,
    0xd8,0x21,0x1d,0xc2,0x61,0x1e,0x66,0x30,0x89,0x3b,0xbc,0x83,0x3b,0xd0,0x43,0x39,
    0xb4,0x03,0x3c,0xbc,0x83,0x3c,0x84,0x03,0x3b,0xcc,0xf0,0x14,0x76,0x60,0x07,0x7b,
    0x68,0x07,0x37,0x68,0x87,0x72,0x68,0x07,0x37,0x80,0x87,0x70,0x90,0x87,0x70,0x60,
    0x07,0x76,0x28,0x07,0x76,0xf8,0x05,0x76,0x78,0x87,0x77,0x80,0x87,0x5f,0x08,0x87,
    0x71,0x18,0x87,0x72,0x98,0x87,0x79,0x98,0x81,0x2c,0xee,0xf0,0x0e,0xee,0xe0,0x0e,
    0xf5,0xc0,0x0e,0xec,0x30,0x03,0x62,0xc8,0xa1,0x1c,0xe4,0xa1,0x1c,0xcc,0xa1,0x1c,
    0xe4,0xa1,0x1c,0xdc,0x61,0x1c,0xca,0x21,0x1c,0xc4,0x81,0x1d,0xca,0x61,0x06,0xd6,
    0x90,0x43,0x39,0xc8,0x43,0x39,0x98,0x43,0x39,0xc8,0x43,0x39,0xb8,0xc3,0x38,0x94,
    0x43,0x38,0x88,0x03,0x3b,0x94,0xc3,0x2f,0xbc,0x83,0x3c,0xfc,0x82,0x3b,0xd4,0x03,
    0x3b,0xb0,0xc3,0x0c,0xc7,0x69,0x87,0x70,0x58,0x87,0x72,0x70,0x83,0x74,0x68,0x07,
    0x78,0x60,0x87,0x74,0x18,0x87,0x74,0xa0,0x87,0x19,0xce,0x53,0x0f,0xee,0x00,0x0f,
    0xf2,0x50,0x0e,0xe4,0x90,0x0e,0xe3,0x40,0x0f,0xe1,0x20,0x0e,0xec,0x50,0x0e,0x33,
    0x20,0x28,0x1d,0xdc,0xc1,0x1e,0xc2,0x41,0x1e,0xd2,0x21,0x1c,0xdc,0x81,0x1e,0xdc,
    0xe0,0x1c,0xe4,0xe1,0x1d,0xea,0x01,0x1e,0x66,0x18,0x51,0x38,0xb0,0x43,0x3a,0x9c,
    0x83,0x3b,0xcc,0x50,0x24,0x76,0x60,0x07,0x7b,0x68,0x07,0x37,0x60,0x87,0x77,0x78,
    0x07,0x78,0x98,0x51,0x4c,0xf4,0x90,0x0f,0xf0,0x50,0x0e,0x33,0x1e,0x6a,0x1e,0xca,
    0x61,0x1c,0xe8,0x21,0x1d,0xde,0xc1,0x1d,0x7e,0x01,0x1e,0xe4,0xa1,0x1c,0xcc,0x21,
    0x1d,0xf0,0x61,0x06,0x54,0x85,0x83,0x38,0xcc,0xc3,0x3b,0xb0,0x43,0x3d,0xd0,0x43,
    0x39,0xfc,0xc2,0x3c,0xe4,0x43,0x3b,0x88,0xc3,0x3b,0xb0,0xc3,0x8c,0xc5,0x0a,0x87,
    0x79,0x98,0x87,0x77,0x18,0x87,0x74,0x08,0x07,0x7a,0x28,0x07,0x72,0x98,0x81,0x5c,
    0xe3,0x10,0x0e,0xec,0xc0,0x0e,0xe5,0x50,0x0e,0xf3,0x30,0x23,0xc1,0xd2,0x41,0x1e,
    0xe4,0xe1,0x17,0xd8,0xe1,0x1d,0xde,0x01,0x1e,0x66,0x50,0x59,0x38,0xa4,0x83,0x3c,
    0xb8,0x81,0x39,0xd4,0x83,0x3b,0x8c,0x03,0x3d,0xa4,0xc3,0x3b,0xb8,0xc3,0x2f,0x9c,
    0x83,0x3c,0xbc,0x43,0x3d,0xc0,0xc3,0x3c,0x00,0x71,0x20,0x00,0x00,0x0b,0x00,0x00,
    0x00,0x26,0xb0,0x01,0x48,0xe4,0x4b,0x00,0xf3,0x2c,0xc4,0x3f,0x11,0xd7,0x44,0x45,
    0xc4,0x6f,0x0f,0x7e,0x85,0x17,0xb7,0x6d,0x00,0x05,0x03,0x20,0x0d,0x6d,0x01,0x0d,
    0x80,0x44,0x3e,0x83,0x5c,0x7e,0x85,0x17,0xb7,0x0d,0x00,0x00,0x00,0x61,0x20,0x00,
    0x00,0x23,0x00,0x00,0x00,0x13,0x04,0x41,0x2c,0x10,0x00,0x00,0x00,0x0c,0x00,0x00,
    0x00,0x74,0x47,0x00,0xc6,0x22,0x80,0x40,0x38,0xe6,0x20,0x06,0xc2,0xa0,0xc8,0xd5,
    0xc0,0x08,0x00,0xbd,0x19,0x00,0x82,0x23,0x00,0x54,0xc7,0x1a,0x80,0x40,0x18,0x6b,
    0x18,0x86,0x81,0xec,0x0c,0x00,0x89,0x19,0x00,0x0a,0x33,0x00,0x04,0x46,0x00,0x00,
    0x00,0x23,0x06,0xca,0x10,0x68,0x8f,0x13,0x29,0x47,0x12,0x0c,0x32,0x04,0xc3,0x31,
    0xc8,0x20,0x10,0x87,0x09,0x81,0x7c,0x06,0x19,0x02,0xe4,0x19,0x64,0x08,0x8a,0x68,
    0xbc,0xc1,0xca,0x3e,0x0a,0xc6,0x1c,0x83,0x12,0x84,0xc1,0x20,0x43,0xb0,0x58,0x23,
    0x06,0x86,0x10,0x94,0x81,0x52,0x04,0xb3,0x0d,0x4e,0x00,0x64,0x10,0x10,0x03,0x00,
    0x00,0x02,0x00,0x00,0x00,0x5b,0x86,0x23,0xe0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
#elif defined(SOKOL_WGPU)
FIXME
#elif defined(SOKOL_DUMMY_BACKEND)
static const char* _sspine_vs_source_dummy = "";
static const char* _sspine_fs_source_dummy = "";
#else
#error "Please define one of SOKOL_GLCORE33, SOKOL_GLES2, SOKOL_GLES3, SOKOL_D3D11, SOKOL_METAL, SOKOL_WGPU or SOKOL_DUMMY_BACKEND!"
#endif

#define _sspine_def(val, def) (((val) == 0) ? (def) : (val))
#define _SSPINE_INIT_COOKIE (0xABBAABBA)
#define _SSPINE_INVALID_SLOT_INDEX (0)
#define _SSPINE_MAX_STACK_DEPTH (64)
#define _SSPINE_DEFAULT_CONTEXT_POOL_SIZE (4)
#define _SSPINE_DEFAULT_ATLAS_POOL_SIZE (64)
#define _SSPINE_DEFAULT_SKELETON_POOL_SIZE (64)
#define _SSPINE_DEFAULT_INSTANCE_POOL_SIZE (1024)
#define _SSPINE_DEFAULT_MAX_VERTICES (1<<16)
#define _SSPINE_DEFAULT_MAX_COMMANDS (1<<14)
#define _SSPINE_SLOT_SHIFT (16)
#define _SSPINE_MAX_POOL_SIZE (1<<_SSPINE_SLOT_SHIFT)
#define _SSPINE_SLOT_MASK (_SSPINE_MAX_POOL_SIZE-1)

typedef struct {
    float mvp[16];
} _sspine_vsparams_t;

typedef struct {
    float pma;
    uint8_t _pad[12];
} _sspine_fsparams_t;

typedef struct {
    uint32_t id;
    sspine_resource_state state;
} _sspine_slot_t;

typedef struct {
    int size;
    int queue_top;
    uint32_t* gen_ctrs;
    int* free_queue;
} _sspine_pool_t;

typedef struct {
    _sspine_slot_t slot;
    sspine_atlas_overrides overrides;
    spAtlas* sp_atlas;
    int num_pages;
} _sspine_atlas_t;

typedef struct {
    _sspine_pool_t pool;
    _sspine_atlas_t* items;
} _sspine_atlas_pool_t;

typedef struct {
    uint32_t id;
    _sspine_atlas_t* ptr;
} _sspine_atlas_ref_t;

typedef struct {
    _sspine_slot_t slot;
    _sspine_atlas_ref_t atlas;
    spSkeletonData* sp_skel_data;
    spAnimationStateData* sp_anim_data;
    struct {
        int num;
        sspine_vec2* ptr;
    } tform_buf;
} _sspine_skeleton_t;

typedef struct {
    _sspine_pool_t pool;
    _sspine_skeleton_t* items;
} _sspine_skeleton_pool_t;

typedef struct {
    uint32_t id;
    _sspine_skeleton_t* ptr;
} _sspine_skeleton_ref_t;

typedef struct {
    _sspine_slot_t slot;
    _sspine_atlas_ref_t atlas;
    _sspine_skeleton_ref_t skel;
    spSkeleton* sp_skel;
    spAnimationState* sp_anim_state;
    spSkeletonClipping* sp_clip;
} _sspine_instance_t;

typedef struct {
    _sspine_pool_t pool;
    _sspine_instance_t* items;
} _sspine_instance_pool_t;

typedef struct {
    sspine_vec2 pos;
    sspine_vec2 uv;
    uint32_t color;
} _sspine_vertex_t;

typedef struct {
    _sspine_vertex_t* ptr;
    int index;
} _sspine_alloc_vertices_result_t;

typedef struct {
    uint32_t* ptr;
    int index;
} _sspine_alloc_indices_result_t;

typedef struct {
    int layer;
    sg_pipeline pip;
    sg_image img;
    float pma; // pma = 0.0: use texture color as is, pma = 1.0: multiply texture rgb by texture alpha in fragment shader
    int base_element;
    int num_elements;
} _sspine_command_t;

typedef struct {
    _sspine_slot_t slot;
    float transform[16];
    struct {
        int num;
        int cur;
        uint32_t rewind_frame_count;
        _sspine_vertex_t* ptr;
    } vertices;
    struct {
        int num;
        int cur;
        uint32_t rewind_frame_count;
        uint32_t* ptr;
    } indices;
    struct {
        int num;
        int cur;
        uint32_t rewind_frame_count;
        _sspine_command_t* ptr;
    } commands;
    uint32_t update_frame_count;
    sg_buffer vbuf;
    sg_buffer ibuf;
    struct {
        sg_pipeline normal_additive;
        sg_pipeline multiply;
    } pip;
    sg_bindings bind;
} _sspine_context_t;

typedef struct {
    _sspine_pool_t pool;
    _sspine_context_t* items;
} _sspine_context_pool_t;

typedef struct {
    uint32_t init_cookie;
    uint32_t frame_count;
    sspine_desc desc;
    sspine_context def_ctx_id;
    sspine_context cur_ctx_id;
    _sspine_context_t* cur_ctx;   // may be 0!
    sg_shader shd;
    _sspine_context_pool_t context_pool;
    _sspine_atlas_pool_t atlas_pool;
    _sspine_skeleton_pool_t skeleton_pool;
    _sspine_instance_pool_t instance_pool;
} _sspine_t;
static _sspine_t _sspine;

// dummy spine-c platform implementation functions
void _spAtlasPage_createTexture(spAtlasPage* self, const char* path) {
    // nothing to do here
    (void)self; (void)path;
}

char* _spUtil_readFile(const char* path, int* length) {
    (void)path;
    *length = 0;
    return 0;
}

//=== MEMORY MANAGEMENT FUNCTIONS ==============================================
static void _sspine_clear(void* ptr, size_t size) {
    SOKOL_ASSERT(ptr && (size > 0));
    memset(ptr, 0, size);
}

static void* _sspine_malloc(size_t size) {
    SOKOL_ASSERT(size > 0);
    void* ptr;
    if (_sspine.desc.allocator.alloc) {
        ptr = _sspine.desc.allocator.alloc(size, _sspine.desc.allocator.user_data);
    }
    else {
        ptr = malloc(size);
    }
    SOKOL_ASSERT(ptr);
    return ptr;
}

static void* _sspine_malloc_clear(size_t size) {
    void* ptr = _sspine_malloc(size);
    _sspine_clear(ptr, size);
    return ptr;
}

static void _sspine_free(void* ptr) {
    if (_sspine.desc.allocator.free) {
        _sspine.desc.allocator.free(ptr, _sspine.desc.allocator.user_data);
    }
    else {
        free(ptr);
    }
}

static bool _sspine_atlas_ref_valid(const _sspine_atlas_ref_t* ref) {
    return ref->ptr && (ref->ptr->slot.id == ref->id);
}

static bool _sspine_skeleton_ref_valid(const _sspine_skeleton_ref_t* ref) {
    return ref->ptr && (ref->ptr->slot.id == ref->id);
}

//=== HANDLE POOL FUNCTIONS ====================================================
static void _sspine_init_pool(_sspine_pool_t* pool, int num) {
    SOKOL_ASSERT(pool && (num >= 1));
    // slot 0 is reserved for the 'invalid id', so bump the pool size by 1
    pool->size = num + 1;
    pool->queue_top = 0;
    // generation counters indexable by pool slot index, slot 0 is reserved
    size_t gen_ctrs_size = sizeof(uint32_t) * (size_t)pool->size;
    pool->gen_ctrs = (uint32_t*) _sspine_malloc_clear(gen_ctrs_size);
    // it's not a bug to only reserve 'num' here
    pool->free_queue = (int*) _sspine_malloc_clear(sizeof(int) * (size_t)num);
    // never allocate the zero-th pool item since the invalid id is 0
    for (int i = pool->size-1; i >= 1; i--) {
        pool->free_queue[pool->queue_top++] = i;
    }
}

static void _sspine_discard_pool(_sspine_pool_t* pool) {
    SOKOL_ASSERT(pool);
    SOKOL_ASSERT(pool->free_queue);
    _sspine_free(pool->free_queue);
    pool->free_queue = 0;
    SOKOL_ASSERT(pool->gen_ctrs);
    _sspine_free(pool->gen_ctrs);
    pool->gen_ctrs = 0;
    pool->size = 0;
    pool->queue_top = 0;
}

static int _sspine_pool_alloc_index(_sspine_pool_t* pool) {
    SOKOL_ASSERT(pool);
    SOKOL_ASSERT(pool->free_queue);
    if (pool->queue_top > 0) {
        int slot_index = pool->free_queue[--pool->queue_top];
        SOKOL_ASSERT((slot_index > 0) && (slot_index < pool->size));
        return slot_index;
    }
    else {
        // pool exhausted
        return _SSPINE_INVALID_SLOT_INDEX;
    }
}

static void _sspine_pool_free_index(_sspine_pool_t* pool, int slot_index) {
    SOKOL_ASSERT((slot_index > _SSPINE_INVALID_SLOT_INDEX) && (slot_index < pool->size));
    SOKOL_ASSERT(pool);
    SOKOL_ASSERT(pool->free_queue);
    SOKOL_ASSERT(pool->queue_top < pool->size);
    #ifdef SOKOL_DEBUG
    // debug check against double-free
    for (int i = 0; i < pool->queue_top; i++) {
        SOKOL_ASSERT(pool->free_queue[i] != slot_index);
    }
    #endif
    pool->free_queue[pool->queue_top++] = slot_index;
    SOKOL_ASSERT(pool->queue_top <= (pool->size-1));
}

/* initiailize a pool slot:
    - bump the slot's generation counter
    - create a resource id from the generation counter and slot index
    - set the slot's id to this id
    - set the slot's state to ALLOC
    - return the handle id
*/
static uint32_t _sspine_slot_init(_sspine_pool_t* pool, _sspine_slot_t* slot, int slot_index) {
    /* FIXME: add handling for an overflowing generation counter,
       for now, just overflow (another option is to disable
       the slot)
    */
    SOKOL_ASSERT(pool && pool->gen_ctrs);
    SOKOL_ASSERT((slot_index > _SSPINE_INVALID_SLOT_INDEX) && (slot_index < pool->size));
    SOKOL_ASSERT((slot->state == SSPINE_RESOURCESTATE_INITIAL) && (slot->id == SSPINE_INVALID_ID));
    uint32_t ctr = ++pool->gen_ctrs[slot_index];
    slot->id = (ctr<<_SSPINE_SLOT_SHIFT)|(slot_index & _SSPINE_SLOT_MASK);
    slot->state = SSPINE_RESOURCESTATE_ALLOC;
    return slot->id;
}

// extract slot index from id
static int _sspine_slot_index(uint32_t id) {
    int slot_index = (int) (id & _SSPINE_SLOT_MASK);
    SOKOL_ASSERT(_SSPINE_INVALID_SLOT_INDEX != slot_index);
    return slot_index;
}

static void _sspine_init_item_pool(_sspine_pool_t* pool, int pool_size, void** items_ptr, size_t item_size_bytes) {
    // NOTE: the pools will have an additional item, since slot 0 is reserved
    SOKOL_ASSERT(pool && (pool->size == 0));
    SOKOL_ASSERT((pool_size > 0) && (pool_size < _SSPINE_MAX_POOL_SIZE));
    SOKOL_ASSERT(items_ptr && (*items_ptr == 0));
    SOKOL_ASSERT(item_size_bytes > 0);
    _sspine_init_pool(pool, pool_size);
    const size_t pool_size_bytes = item_size_bytes * (size_t)pool->size;
    *items_ptr = _sspine_malloc_clear(pool_size_bytes);
}

static void _sspine_discard_item_pool(_sspine_pool_t* pool, void** items_ptr) {
    SOKOL_ASSERT(pool && (pool->size != 0));
    SOKOL_ASSERT(items_ptr && (*items_ptr != 0));
    _sspine_free(*items_ptr); *items_ptr = 0;
    _sspine_discard_pool(pool);
}

//== CONTEXT POOL FUNCTIONS ====================================================
static void _sspine_setup_context_pool(int pool_size) {
    _sspine_context_pool_t* p = &_sspine.context_pool;
    _sspine_init_item_pool(&p->pool, pool_size, (void**)&p->items, sizeof(_sspine_context_t));
}

static void _sspine_discard_context_pool(void) {
    _sspine_context_pool_t* p = &_sspine.context_pool;
    _sspine_discard_item_pool(&p->pool, (void**)&p->items);
}

static sspine_context _sspine_make_context_handle(uint32_t id) {
    sspine_context handle = { id };
    return handle;
}

static _sspine_context_t* _sspine_context_at(uint32_t id) {
    SOKOL_ASSERT(SSPINE_INVALID_ID != id);
    const _sspine_context_pool_t* p = &_sspine.context_pool;
    int slot_index = _sspine_slot_index(id);
    SOKOL_ASSERT((slot_index > _SSPINE_INVALID_SLOT_INDEX) && (slot_index < p->pool.size));
    return &p->items[slot_index];
}

static _sspine_context_t* _sspine_lookup_context(uint32_t id) {
    if (SSPINE_INVALID_ID != id) {
        _sspine_context_t* ctx = _sspine_context_at(id);
        if (ctx->slot.id == id) {
            return ctx;
        }
    }
    return 0;
}

static sspine_context _sspine_alloc_context(void) {
    _sspine_context_pool_t* p = &_sspine.context_pool;
    sspine_context res;
    int slot_index = _sspine_pool_alloc_index(&p->pool);
    if (_SSPINE_INVALID_SLOT_INDEX != slot_index) {
        uint32_t id = _sspine_slot_init(&p->pool, &p->items[slot_index].slot, slot_index);
        res = _sspine_make_context_handle(id);
    }
    else {
        // pool exhausted
        res = _sspine_make_context_handle(SSPINE_INVALID_ID);
    }
    return res;
}

static sspine_resource_state _sspine_init_context(_sspine_context_t* ctx, const sspine_context_desc* desc) {
    SOKOL_ASSERT(ctx && (ctx->slot.state == SSPINE_RESOURCESTATE_ALLOC));
    SOKOL_ASSERT(desc);

    // setup vertex, index and command storage
    ctx->vertices.num = desc->max_vertices;
    ctx->indices.num = ctx->vertices.num * 3;
    ctx->commands.num = desc->max_commands;

    const size_t vbuf_size = (size_t)ctx->vertices.num * sizeof(_sspine_vertex_t);
    const size_t ibuf_size = (size_t)ctx->indices.num * sizeof(uint32_t);
    const size_t cbuf_size = (size_t)ctx->commands.num * sizeof(_sspine_command_t);

    ctx->vertices.ptr = (_sspine_vertex_t*) _sspine_malloc(vbuf_size);
    ctx->indices.ptr = (uint32_t*) _sspine_malloc(ibuf_size);
    ctx->commands.ptr = (_sspine_command_t*) _sspine_malloc(cbuf_size);

    sg_buffer_desc vbuf_desc;
    _sspine_clear(&vbuf_desc, sizeof(vbuf_desc));
    vbuf_desc.type = SG_BUFFERTYPE_VERTEXBUFFER;
    vbuf_desc.usage = SG_USAGE_STREAM;
    vbuf_desc.size = vbuf_size;
    vbuf_desc.label = "sspine-vbuf";
    ctx->vbuf = sg_make_buffer(&vbuf_desc);
    ctx->bind.vertex_buffers[0] = ctx->vbuf;

    sg_buffer_desc ibuf_desc;
    _sspine_clear(&ibuf_desc, sizeof(ibuf_desc));
    ibuf_desc.type = SG_BUFFERTYPE_INDEXBUFFER;
    ibuf_desc.usage = SG_USAGE_STREAM;
    ibuf_desc.size = ibuf_size;
    ibuf_desc.label = "sspine-ibuf";
    ctx->ibuf = sg_make_buffer(&ibuf_desc);
    ctx->bind.index_buffer = ctx->ibuf;

    // for blend modes, see: https://wiki.libsdl.org/SDL_BlendMode
    //
    // NOTE: we're configuring the blend mode for premultiplied alpha,
    // and then do the premultiplication in the fragment shader
    // if needed
    sg_pipeline_desc pip_desc;
    _sspine_clear(&pip_desc, sizeof(pip_desc));
    pip_desc.shader = _sspine.shd;
    pip_desc.layout.buffers[0].stride = sizeof(_sspine_vertex_t);
    pip_desc.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT2;
    pip_desc.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT2;
    pip_desc.layout.attrs[2].format = SG_VERTEXFORMAT_UBYTE4N;
    pip_desc.index_type = SG_INDEXTYPE_UINT32;
    pip_desc.sample_count = desc->sample_count;
    pip_desc.depth.pixel_format = desc->depth_format;
    pip_desc.colors[0].pixel_format = desc->color_format;
    pip_desc.colors[0].write_mask = desc->color_write_mask;
    pip_desc.colors[0].blend.enabled = true;
    pip_desc.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_ONE;
    pip_desc.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    pip_desc.colors[0].blend.src_factor_alpha = SG_BLENDFACTOR_ONE;
    pip_desc.colors[0].blend.dst_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    pip_desc.label = "sspine-pip-normal/additive";
    ctx->pip.normal_additive = sg_make_pipeline(&pip_desc);

    pip_desc.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_ZERO;
    pip_desc.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_SRC_COLOR;
    pip_desc.colors[0].blend.src_factor_alpha = SG_BLENDFACTOR_ZERO;
    pip_desc.colors[0].blend.dst_factor_alpha = SG_BLENDFACTOR_ONE;
    pip_desc.label = "sspine-pip-multiply";
    ctx->pip.multiply = sg_make_pipeline(&pip_desc);

    return SSPINE_RESOURCESTATE_VALID;
}

static void _sspine_deinit_context(_sspine_context_t* ctx) {
    // NOTE: it's ok to call sg_destroy functions with invalid handles
    sg_destroy_pipeline(ctx->pip.normal_additive);
    sg_destroy_pipeline(ctx->pip.multiply);
    sg_destroy_buffer(ctx->ibuf);
    sg_destroy_buffer(ctx->vbuf);
    if (ctx->commands.ptr) {
        _sspine_free(ctx->commands.ptr);
        ctx->commands.ptr = 0;
    }
    if (ctx->indices.ptr) {
        _sspine_free(ctx->indices.ptr);
        ctx->indices.ptr = 0;
    }
    if (ctx->vertices.ptr) {
        _sspine_free(ctx->vertices.ptr);
        ctx->vertices.ptr = 0;
    }
}

static void _sspine_destroy_context(sspine_context ctx_id) {
    _sspine_context_t* ctx = _sspine_lookup_context(ctx_id.id);
    if (ctx) {
        _sspine_deinit_context(ctx);
        _sspine_context_pool_t* p = &_sspine.context_pool;
        _sspine_clear(ctx, sizeof(_sspine_context_t));
        _sspine_pool_free_index(&p->pool, _sspine_slot_index(ctx_id.id));
    }
}

static void _sspine_destroy_all_contexts(void) {
    _sspine_context_pool_t* p = &_sspine.context_pool;
    for (int i = 0; i < p->pool.size; i++) {
        _sspine_context_t* ctx = &p->items[i];
        _sspine_destroy_context(_sspine_make_context_handle(ctx->slot.id));
    }
}

static sspine_context_desc _sspine_context_desc_defaults(const sspine_context_desc* desc) {
    sspine_context_desc res = *desc;
    res.max_vertices = _sspine_def(desc->max_vertices, _SSPINE_DEFAULT_MAX_VERTICES);
    res.max_commands = _sspine_def(desc->max_commands, _SSPINE_DEFAULT_MAX_COMMANDS);
    return res;
}

static bool _sspine_is_default_context(sspine_context ctx_id) {
    return ctx_id.id == 0x00010001;
}

//=== ATLAS POOL FUNCTIONS =====================================================
static void _sspine_setup_atlas_pool(int pool_size) {
    _sspine_atlas_pool_t* p = &_sspine.atlas_pool;
    _sspine_init_item_pool(&p->pool, pool_size, (void**)&p->items, sizeof(_sspine_atlas_t));
}

static void _sspine_discard_atlas_pool(void) {
    _sspine_atlas_pool_t* p = &_sspine.atlas_pool;
    _sspine_discard_item_pool(&p->pool, (void**)&p->items);
}

static sspine_atlas _sspine_make_atlas_handle(uint32_t id) {
    sspine_atlas handle = { id };
    return handle;
}

static _sspine_atlas_t* _sspine_atlas_at(uint32_t id) {
    SOKOL_ASSERT(SSPINE_INVALID_ID != id);
    const _sspine_atlas_pool_t* p = &_sspine.atlas_pool;
    int slot_index = _sspine_slot_index(id);
    SOKOL_ASSERT((slot_index > _SSPINE_INVALID_SLOT_INDEX) && (slot_index < p->pool.size));
    return &p->items[slot_index];
}

static _sspine_atlas_t* _sspine_lookup_atlas(uint32_t id) {
    if (SSPINE_INVALID_ID != id) {
        _sspine_atlas_t* atlas = _sspine_atlas_at(id);
        if (atlas->slot.id == id) {
            return atlas;
        }
    }
    return 0;
}

static sspine_atlas _sspine_alloc_atlas(void) {
    _sspine_atlas_pool_t* p = &_sspine.atlas_pool;
    sspine_atlas res;
    int slot_index = _sspine_pool_alloc_index(&p->pool);
    if (_SSPINE_INVALID_SLOT_INDEX != slot_index) {
        uint32_t id = _sspine_slot_init(&p->pool, &p->items[slot_index].slot, slot_index);
        res = _sspine_make_atlas_handle(id);
    }
    else {
        // pool exhausted
        res = _sspine_make_atlas_handle(SSPINE_INVALID_ID);
    }
    return res;
}

void _spAtlasPage_disposeTexture(spAtlasPage* self) {
    if (self->rendererObject != 0) {
        const sg_image img = { (uint32_t)(uintptr_t)self->rendererObject };
        sg_destroy_image(img);
    }
}

static sspine_resource_state _sspine_init_atlas(_sspine_atlas_t* atlas, const sspine_atlas_desc* desc) {
    SOKOL_ASSERT(atlas && (atlas->slot.state == SSPINE_RESOURCESTATE_ALLOC));
    SOKOL_ASSERT(desc);
    SOKOL_ASSERT(atlas->sp_atlas == 0);

    if ((desc->data.ptr == 0) || (desc->data.size == 0)) {
        return SSPINE_RESOURCESTATE_FAILED;
    }
    atlas->overrides = desc->override;

    // NOTE: Spine doesn't detect when invalid or corrupt data is passed here,
    // not much we can do about this...
    atlas->sp_atlas = spAtlas_create((const char*)desc->data.ptr, (int)desc->data.size, "", 0);
    if (0 == atlas->sp_atlas) {
        return SSPINE_RESOURCESTATE_FAILED;
    }

    // allocate a sokol-gfx image handle for each page, but the actual image initialization
    // needs to be delegated to the application
    for (spAtlasPage* page = atlas->sp_atlas->pages; page != 0; page = page->next) {
        atlas->num_pages++;
        const sg_image img = sg_alloc_image();
        if (sg_query_image_state(img) != SG_RESOURCESTATE_ALLOC) {
            return SSPINE_RESOURCESTATE_FAILED;
        }
        page->rendererObject = (void*)(uintptr_t)img.id;
        if (desc->override.premul_alpha_enabled) {
            // NOTE: -1 is spine-c convention for 'true'
            page->pma = -1;
        }
        else if (desc->override.premul_alpha_disabled) {
            page->pma = 0;
        }
    }
    return SSPINE_RESOURCESTATE_VALID;
}

static void _sspine_deinit_atlas(_sspine_atlas_t* atlas) {
    if (atlas->sp_atlas) {
        spAtlas_dispose(atlas->sp_atlas);
        atlas->sp_atlas = 0;
    }
}

static void _sspine_destroy_atlas(sspine_atlas atlas_id) {
    _sspine_atlas_t* atlas = _sspine_lookup_atlas(atlas_id.id);
    if (atlas) {
        _sspine_deinit_atlas(atlas);
        _sspine_atlas_pool_t* p = &_sspine.atlas_pool;
        _sspine_clear(atlas, sizeof(_sspine_atlas_t));
        _sspine_pool_free_index(&p->pool, _sspine_slot_index(atlas_id.id));
    }
}

static void _sspine_destroy_all_atlases(void) {
    _sspine_atlas_pool_t* p = &_sspine.atlas_pool;
    for (int i = 0; i < p->pool.size; i++) {
        _sspine_atlas_t* atlas = &p->items[i];
        _sspine_destroy_atlas(_sspine_make_atlas_handle(atlas->slot.id));
    }
}

static sspine_atlas_desc _sspine_atlas_desc_defaults(const sspine_atlas_desc* desc) {
    sspine_atlas_desc res = *desc;
    return res;
}

//=== SKELETON POOL FUNCTIONS ==================================================
static void _sspine_setup_skeleton_pool(int pool_size) {
    _sspine_skeleton_pool_t* p = &_sspine.skeleton_pool;
    _sspine_init_item_pool(&p->pool, pool_size, (void**)&p->items, sizeof(_sspine_skeleton_t));
}

static void _sspine_discard_skeleton_pool(void) {
    _sspine_skeleton_pool_t* p = &_sspine.skeleton_pool;
    _sspine_discard_item_pool(&p->pool, (void**)&p->items);
}

static sspine_skeleton _sspine_make_skeleton_handle(uint32_t id) {
    sspine_skeleton handle = { id };
    return handle;
}

static _sspine_skeleton_t* _sspine_skeleton_at(uint32_t id) {
    SOKOL_ASSERT(SSPINE_INVALID_ID != id);
    const _sspine_skeleton_pool_t* p = &_sspine.skeleton_pool;
    int slot_index = _sspine_slot_index(id);
    SOKOL_ASSERT((slot_index > _SSPINE_INVALID_SLOT_INDEX) && (slot_index < p->pool.size));
    return &p->items[slot_index];
}

static _sspine_skeleton_t* _sspine_lookup_skeleton(uint32_t id) {
    if (SSPINE_INVALID_ID != id) {
        _sspine_skeleton_t* skeleton = _sspine_skeleton_at(id);
        if (skeleton->slot.id == id) {
            return skeleton;
        }
    }
    return 0;
}

static sspine_skeleton _sspine_alloc_skeleton(void) {
    _sspine_skeleton_pool_t* p = &_sspine.skeleton_pool;
    sspine_skeleton res;
    int slot_index = _sspine_pool_alloc_index(&p->pool);
    if (_SSPINE_INVALID_SLOT_INDEX != slot_index) {
        uint32_t id = _sspine_slot_init(&p->pool, &p->items[slot_index].slot, slot_index);
        res = _sspine_make_skeleton_handle(id);
    }
    else {
        // pool exhausted
        res = _sspine_make_skeleton_handle(SSPINE_INVALID_ID);
    }
    return res;
}

static sspine_resource_state _sspine_init_skeleton(_sspine_skeleton_t* skeleton, const sspine_skeleton_desc* desc) {
    SOKOL_ASSERT(skeleton && (skeleton->slot.state = SSPINE_RESOURCESTATE_ALLOC));
    SOKOL_ASSERT(desc);

    if ((0 == desc->json_data) && ((0 == desc->binary_data.ptr) || (0 == desc->binary_data.size))) {
        return SSPINE_RESOURCESTATE_FAILED;
    }

    skeleton->atlas.id = desc->atlas.id;
    skeleton->atlas.ptr = _sspine_lookup_atlas(skeleton->atlas.id);
    if (!_sspine_atlas_ref_valid(&skeleton->atlas)) {
        return SSPINE_RESOURCESTATE_FAILED;
    }
    _sspine_atlas_t* atlas = skeleton->atlas.ptr;
    if (SSPINE_RESOURCESTATE_VALID != atlas->slot.state) {
        return SSPINE_RESOURCESTATE_FAILED;
    }
    SOKOL_ASSERT(atlas->sp_atlas);

    if (desc->json_data) {
        spSkeletonJson* skel_json = spSkeletonJson_create(atlas->sp_atlas);
        SOKOL_ASSERT(skel_json);
        skel_json->scale = desc->prescale;
        skeleton->sp_skel_data = spSkeletonJson_readSkeletonData(skel_json, desc->json_data);
        spSkeletonJson_dispose(skel_json); skel_json = 0;
        if (0 == skeleton->sp_skel_data) {
            return SSPINE_RESOURCESTATE_FAILED;
        }
    }
    else {
        spSkeletonBinary* skel_bin = spSkeletonBinary_create(atlas->sp_atlas);
        SOKOL_ASSERT(skel_bin);
        skel_bin->scale = desc->prescale;
        skeleton->sp_skel_data = spSkeletonBinary_readSkeletonData(skel_bin, desc->binary_data.ptr, (int)desc->binary_data.size);
        spSkeletonBinary_dispose(skel_bin); skel_bin = 0;
        if (0 == skeleton->sp_skel_data) {
            return SSPINE_RESOURCESTATE_FAILED;
        }
    }
    SOKOL_ASSERT(skeleton->sp_skel_data);

    skeleton->sp_anim_data = spAnimationStateData_create(skeleton->sp_skel_data);
    SOKOL_ASSERT(skeleton->sp_anim_data);
    skeleton->sp_anim_data->defaultMix = desc->anim_default_mix;

    // get the max number of vertices in any mesh attachment
    int max_vertex_count = 4;   // number of vertices in a 'region attachment' (a 2-triangle quad)
    const spSkeletonData* sp_skel_data = skeleton->sp_skel_data;
    for (int skinIndex = 0; skinIndex < sp_skel_data->skinsCount; skinIndex++) {
        const spSkin* sp_skin = sp_skel_data->skins[skinIndex];
        const spSkinEntry* skin_entry = spSkin_getAttachments(sp_skin);
        if (skin_entry) do {
            if (skin_entry->attachment) {
                if (skin_entry->attachment->type == SP_ATTACHMENT_MESH) {
                    const spMeshAttachment* mesh_attachment = (spMeshAttachment*)skin_entry->attachment;
                    // worldVerticesLength is number of floats
                    SOKOL_ASSERT((mesh_attachment->super.worldVerticesLength & 1) == 0);
                    const int num_vertices = mesh_attachment->super.worldVerticesLength / 2;
                    if (num_vertices > max_vertex_count) {
                        max_vertex_count = num_vertices;
                    }
                }
            }
        } while ((skin_entry = skin_entry->next) != 0);
    }

    // allocate a shared vertex transform buffer (big enough to hold vertices for biggest mesh attachment)
    skeleton->tform_buf.num = max_vertex_count;
    skeleton->tform_buf.ptr = (sspine_vec2*) _sspine_malloc((size_t)skeleton->tform_buf.num * sizeof(sspine_vec2));

    return SSPINE_RESOURCESTATE_VALID;
}

static void _sspine_deinit_skeleton(_sspine_skeleton_t* skeleton) {
    if (skeleton->tform_buf.ptr) {
        _sspine_free(skeleton->tform_buf.ptr);
        skeleton->tform_buf.ptr = 0;
    }
    if (skeleton->sp_anim_data) {
        spAnimationStateData_dispose(skeleton->sp_anim_data);
        skeleton->sp_anim_data = 0;
    }
    if (skeleton->sp_skel_data) {
        spSkeletonData_dispose(skeleton->sp_skel_data);
        skeleton->sp_skel_data = 0;
    }
}

static void _sspine_destroy_skeleton(sspine_skeleton skeleton_id) {
    _sspine_skeleton_t* skeleton = _sspine_lookup_skeleton(skeleton_id.id);
    if (skeleton) {
        _sspine_deinit_skeleton(skeleton);
        _sspine_skeleton_pool_t* p = &_sspine.skeleton_pool;
        _sspine_clear(skeleton, sizeof(_sspine_skeleton_t));
        _sspine_pool_free_index(&p->pool, _sspine_slot_index(skeleton_id.id));
    }
}

static void _sspine_destroy_all_skeletons(void) {
    _sspine_skeleton_pool_t* p = &_sspine.skeleton_pool;
    for (int i = 0; i < p->pool.size; i++) {
        _sspine_skeleton_t* skeleton = &p->items[i];
        _sspine_destroy_skeleton(_sspine_make_skeleton_handle(skeleton->slot.id));
    }
}

static sspine_skeleton_desc _sspine_skeleton_desc_defaults(const sspine_skeleton_desc* desc) {
    sspine_skeleton_desc res = *desc;
    res.prescale = _sspine_def(desc->prescale, 1.0f);
    res.anim_default_mix = _sspine_def(desc->anim_default_mix, 0.2f);
    return res;
}

//=== INSTANCE POOL FUNCTIONS ==================================================
static void _sspine_setup_instance_pool(int pool_size) {
    _sspine_instance_pool_t* p = &_sspine.instance_pool;
    _sspine_init_item_pool(&p->pool, pool_size, (void**)&p->items, sizeof(_sspine_instance_t));
}

static void _sspine_discard_instance_pool(void) {
    _sspine_instance_pool_t* p = &_sspine.instance_pool;
    _sspine_discard_item_pool(&p->pool, (void**)&p->items);
}

static sspine_instance _sspine_make_instance_handle(uint32_t id) {
    sspine_instance handle = { id };
    return handle;
}

static _sspine_instance_t* _sspine_instance_at(uint32_t id) {
    SOKOL_ASSERT(SSPINE_INVALID_ID != id);
    const _sspine_instance_pool_t* p = &_sspine.instance_pool;
    int slot_index = _sspine_slot_index(id);
    SOKOL_ASSERT((slot_index > _SSPINE_INVALID_SLOT_INDEX) && (slot_index < p->pool.size));
    return &p->items[slot_index];
}

static _sspine_instance_t* _sspine_lookup_instance(uint32_t id) {
    if (SSPINE_INVALID_ID != id) {
        _sspine_instance_t* instance = _sspine_instance_at(id);
        if (instance->slot.id == id) {
            return instance;
        }
    }
    return 0;
}

static sspine_instance _sspine_alloc_instance(void) {
    _sspine_instance_pool_t* p = &_sspine.instance_pool;
    sspine_instance res;
    int slot_index = _sspine_pool_alloc_index(&p->pool);
    if (_SSPINE_INVALID_SLOT_INDEX != slot_index) {
        uint32_t id = _sspine_slot_init(&p->pool, &p->items[slot_index].slot, slot_index);
        res = _sspine_make_instance_handle(id);
    }
    else {
        // pool exhausted
        res = _sspine_make_instance_handle(SSPINE_INVALID_ID);
    }
    return res;
}

static sspine_resource_state _sspine_init_instance(_sspine_instance_t* instance, const sspine_instance_desc* desc) {
    SOKOL_ASSERT(instance && (instance->slot.state == SSPINE_RESOURCESTATE_ALLOC));
    SOKOL_ASSERT(desc);

    instance->skel.id = desc->skeleton.id;
    instance->skel.ptr = _sspine_lookup_skeleton(instance->skel.id);
    if (!_sspine_skeleton_ref_valid(&instance->skel)) {
        return SSPINE_RESOURCESTATE_FAILED;
    }
    _sspine_skeleton_t* skel = instance->skel.ptr;
    if (SSPINE_RESOURCESTATE_VALID != skel->slot.state) {
        return SSPINE_RESOURCESTATE_FAILED;
    }
    instance->atlas = skel->atlas;
    if (!_sspine_atlas_ref_valid(&instance->atlas)) {
        return SSPINE_RESOURCESTATE_FAILED;
    }
    if (SSPINE_RESOURCESTATE_VALID != instance->atlas.ptr->slot.state) {
        return SSPINE_RESOURCESTATE_FAILED;
    }
    SOKOL_ASSERT(skel->sp_skel_data);
    SOKOL_ASSERT(skel->sp_anim_data);

    instance->sp_skel = spSkeleton_create(skel->sp_skel_data);
    SOKOL_ASSERT(instance->sp_skel);
    instance->sp_anim_state = spAnimationState_create(skel->sp_anim_data);
    SOKOL_ASSERT(instance->sp_anim_state);
    instance->sp_clip = spSkeletonClipping_create();
    SOKOL_ASSERT(instance->sp_clip);

    spSkeleton_setToSetupPose(instance->sp_skel);
    spAnimationState_update(instance->sp_anim_state, 0.0f);
    spAnimationState_apply(instance->sp_anim_state, instance->sp_skel);
    spSkeleton_updateWorldTransform(instance->sp_skel);

    return SSPINE_RESOURCESTATE_VALID;
}

static void _sspine_deinit_instance(_sspine_instance_t* instance) {
    if (instance->sp_clip) {
        spSkeletonClipping_dispose(instance->sp_clip);
        instance->sp_clip = 0;
    }
    if (instance->sp_anim_state) {
        spAnimationState_dispose(instance->sp_anim_state);
        instance->sp_anim_state = 0;
    }
    if (instance->sp_skel) {
        spSkeleton_dispose(instance->sp_skel);
        instance->sp_skel = 0;
    }
}

static void _sspine_destroy_instance(sspine_instance instance_id) {
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance) {
        _sspine_deinit_instance(instance);
        _sspine_instance_pool_t* p = &_sspine.instance_pool;
        _sspine_clear(instance, sizeof(_sspine_instance_t));
        _sspine_pool_free_index(&p->pool, _sspine_slot_index(instance_id.id));
    }
}

static void _sspine_destroy_all_instances(void) {
    _sspine_instance_pool_t* p = &_sspine.instance_pool;
    for (int i = 0; i < p->pool.size; i++) {
        _sspine_instance_t* instance = &p->items[i];
        _sspine_destroy_instance(_sspine_make_instance_handle(instance->slot.id));
    }
}

static sspine_instance_desc _sspine_instance_desc_defaults(const sspine_instance_desc* desc) {
    sspine_instance_desc res = *desc;
    return res;
}

// return sspine_desc with patched defaults
static sspine_desc _sspine_desc_defaults(const sspine_desc* desc) {
    SOKOL_ASSERT((desc->allocator.alloc && desc->allocator.free) || (!desc->allocator.alloc && !desc->allocator.free));
    sspine_desc res = *desc;
    res.max_vertices = _sspine_def(desc->max_vertices, _SSPINE_DEFAULT_MAX_VERTICES);
    res.max_commands = _sspine_def(desc->max_commands, _SSPINE_DEFAULT_MAX_COMMANDS);
    res.context_pool_size = _sspine_def(desc->context_pool_size, _SSPINE_DEFAULT_CONTEXT_POOL_SIZE);
    res.atlas_pool_size = _sspine_def(desc->atlas_pool_size, _SSPINE_DEFAULT_ATLAS_POOL_SIZE);
    res.skeleton_pool_size = _sspine_def(desc->skeleton_pool_size, _SSPINE_DEFAULT_SKELETON_POOL_SIZE);
    res.instance_pool_size = _sspine_def(desc->instance_pool_size, _SSPINE_DEFAULT_INSTANCE_POOL_SIZE);
    return res;
}

static sspine_context_desc _sspine_as_context_desc(const sspine_desc* desc) {
    sspine_context_desc ctx_desc;
    _sspine_clear(&ctx_desc, sizeof(ctx_desc));
    ctx_desc.max_vertices = desc->max_vertices;
    ctx_desc.max_commands = desc->max_commands;
    ctx_desc.color_format = desc->color_format;
    ctx_desc.depth_format = desc->depth_format;
    ctx_desc.sample_count = desc->sample_count;
    ctx_desc.color_write_mask = desc->color_write_mask;
    return ctx_desc;
}

static sg_filter _sspine_as_image_filter(spAtlasFilter filter) {
    switch (filter) {
        case SP_ATLAS_UNKNOWN_FILTER: return _SG_FILTER_DEFAULT;
        case SP_ATLAS_NEAREST: return SG_FILTER_NEAREST;
        case SP_ATLAS_LINEAR: return SG_FILTER_LINEAR;
        case SP_ATLAS_MIPMAP: return SG_FILTER_LINEAR_MIPMAP_NEAREST;
        case SP_ATLAS_MIPMAP_NEAREST_NEAREST: return SG_FILTER_NEAREST_MIPMAP_NEAREST;
        case SP_ATLAS_MIPMAP_LINEAR_NEAREST: return SG_FILTER_LINEAR_MIPMAP_NEAREST;
        case SP_ATLAS_MIPMAP_NEAREST_LINEAR: return SG_FILTER_NEAREST_MIPMAP_LINEAR;
        case SP_ATLAS_MIPMAP_LINEAR_LINEAR: return SG_FILTER_LINEAR_MIPMAP_LINEAR;
    }
}

static sg_wrap _sspine_as_image_wrap(spAtlasWrap wrap) {
    switch (wrap) {
        case SP_ATLAS_MIRROREDREPEAT: return SG_WRAP_MIRRORED_REPEAT;
        case SP_ATLAS_CLAMPTOEDGE: return SG_WRAP_CLAMP_TO_EDGE;
        case SP_ATLAS_REPEAT: return SG_WRAP_REPEAT;
    }
}

static spAtlasPage* _sspine_atlas_page_at(_sspine_atlas_t* atlas, int index) {
    SOKOL_ASSERT(atlas && atlas->sp_atlas);
    SOKOL_ASSERT((index >= 0) && (index < atlas->num_pages));
    int i = 0;
    for (spAtlasPage* page = atlas->sp_atlas->pages; page != 0; page = page->next, i++) {
        if (i == index) {
            return page;
        }
    }
    return 0;
}

static void _sspine_init_image_info(_sspine_atlas_t* atlas, int index, sspine_image_info* info) {
    SOKOL_ASSERT((index >= 0) && (index < atlas->num_pages));
    if ((index < 0) || (index >= atlas->num_pages)) {
        return;
    }
    spAtlasPage* page = _sspine_atlas_page_at(atlas, index);
    SOKOL_ASSERT(page);
    SOKOL_ASSERT(page->name);
    info->image.id = (uint32_t)(uintptr_t)page->rendererObject;
    info->filename = page->name;
    if (atlas->overrides.min_filter != _SG_FILTER_DEFAULT) {
        info->min_filter = atlas->overrides.min_filter;
    }
    else {
        info->min_filter = _sspine_as_image_filter(page->minFilter);
    }
    if (atlas->overrides.mag_filter != _SG_FILTER_DEFAULT) {
        info->mag_filter = atlas->overrides.mag_filter;
    }
    else {
        info->mag_filter = _sspine_as_image_filter(page->magFilter);
    }
    if (atlas->overrides.wrap_u != _SG_WRAP_DEFAULT) {
        info->wrap_u = atlas->overrides.wrap_u;
    }
    else {
        info->wrap_u = _sspine_as_image_wrap(page->uWrap);
    }
    if (atlas->overrides.wrap_v != _SG_WRAP_DEFAULT) {
        info->wrap_v = atlas->overrides.wrap_v;
    }
    else {
        info->wrap_v = _sspine_as_image_wrap(page->vWrap);
    }
    info->width = page->width;
    info->height = page->height;
    // NOTE: override already happened in atlas init
    info->premul_alpha = page->pma != 0;
}

static void _sspine_check_rewind_commands(_sspine_context_t* ctx) {
    if (_sspine.frame_count != ctx->commands.rewind_frame_count) {
        ctx->commands.cur = 0;
        ctx->commands.rewind_frame_count = _sspine.frame_count;
    }
}

static _sspine_command_t* _sspine_next_command(_sspine_context_t* ctx) {
    _sspine_check_rewind_commands(ctx);
    if ((ctx->commands.cur + 1) <= ctx->commands.num) {
        return &(ctx->commands.ptr[ctx->commands.cur++]);
    }
    else {
        return 0;
    }
}

static _sspine_command_t* _sspine_prev_command(_sspine_context_t* ctx) {
    _sspine_check_rewind_commands(ctx);
    if ((ctx->commands.cur > 0) && (ctx->commands.cur <= ctx->commands.num)) {
        return &ctx->commands.ptr[ctx->commands.cur - 1];
    }
    else {
        return 0;
    }
}

static void _sspine_check_rewind_vertices(_sspine_context_t* ctx) {
    if (_sspine.frame_count != ctx->vertices.rewind_frame_count) {
        ctx->vertices.cur = 0;
        ctx->vertices.rewind_frame_count = _sspine.frame_count;
    }
}

static _sspine_alloc_vertices_result_t _sspine_alloc_vertices(_sspine_context_t* ctx, int num) {
    _sspine_check_rewind_vertices(ctx);
    _sspine_alloc_vertices_result_t res;
    _sspine_clear(&res, sizeof(res));
    if ((ctx->vertices.cur + num) <= ctx->vertices.num) {
        res.ptr = &(ctx->vertices.ptr[ctx->vertices.cur]);
        res.index = ctx->vertices.cur;
        ctx->vertices.cur += num;
    }
    return res;
}

static void _sspine_check_rewind_indices(_sspine_context_t* ctx) {
    if (_sspine.frame_count != ctx->indices.rewind_frame_count) {
        ctx->indices.cur = 0;
        ctx->indices.rewind_frame_count = _sspine.frame_count;
    }
}

static _sspine_alloc_indices_result_t _sspine_alloc_indices(_sspine_context_t* ctx, int num) {
    _sspine_check_rewind_indices(ctx);
    _sspine_alloc_indices_result_t res;
    _sspine_clear(&res, sizeof(res));
    if ((ctx->indices.cur + num) <= ctx->indices.num) {
        res.ptr = &(ctx->indices.ptr[ctx->indices.cur]);
        res.index = ctx->indices.cur;
        ctx->indices.cur += num;
    }
    return res;
}

static bool _sspine_instance_deps_valid(_sspine_instance_t* instance) {
    if (!_sspine_atlas_ref_valid(&instance->atlas)) {
        return false;
    }
    if (!_sspine_skeleton_ref_valid(&instance->skel)) {
        return false;
    }
    return true;
}

static void _sspine_draw_instance(_sspine_context_t* ctx, _sspine_instance_t* instance, int layer) {
    SOKOL_ASSERT(instance->sp_skel);
    SOKOL_ASSERT(instance->sp_anim_state);
    SOKOL_ASSERT(instance->sp_clip);
    SOKOL_ASSERT(_sspine_instance_deps_valid(instance));

    // see: https://github.com/EsotericSoftware/spine-runtimes/blob/4.1/spine-sdl/src/spine-sdl-c.c
    const spSkeleton* sp_skel = instance->sp_skel;
    float* tform_buf = (float*)instance->skel.ptr->tform_buf.ptr;
    const int max_tform_buf_verts = instance->skel.ptr->tform_buf.num;
    const int tform_buf_stride = 2; // each element is 2 floats
    spSkeletonClipping* sp_clip = instance->sp_clip;
    for (int i = 0; i < sp_skel->slotsCount; i++) {
        spSlot* sp_slot = sp_skel->drawOrder[i];
        if (!sp_slot->attachment) {
            spSkeletonClipping_clipEnd(sp_clip, sp_slot);
            continue;
        }

        // early out if the slot alpha is 0 or the bone is not active
        // FIXME: does alpha 0 actually mean 'invisible' for all blend modes?
        if ((sp_slot->color.a == 0) || (!sp_slot->bone->active)) {
            spSkeletonClipping_clipEnd(sp_clip, sp_slot);
            continue;
        }

        int num_vertices = 0;
        float* uvs = 0;
        float* vertices = 0;
        int num_indices = 0;
        const uint16_t* indices = 0;
        const spColor* att_color = 0;
        sg_image img = { SG_INVALID_ID };
        bool premul_alpha = false;
        if (sp_slot->attachment->type == SP_ATTACHMENT_REGION) {
            static const uint16_t quad_indices[] = { 0, 1, 2, 2, 3, 0 };
            spRegionAttachment* region = (spRegionAttachment*)sp_slot->attachment;
            att_color = &region->color;
            // FIXME(?) early out if the slot alpha is 0
            if (att_color->a == 0) {
                spSkeletonClipping_clipEnd(sp_clip, sp_slot);
                continue;
            }
            spRegionAttachment_computeWorldVertices(region, sp_slot, tform_buf, 0, tform_buf_stride);
            vertices = tform_buf;
            num_vertices = 4;
            indices = &quad_indices[0];
            num_indices = 6;
            uvs = region->uvs;
            const spAtlasPage* sp_page = ((spAtlasRegion*)region->rendererObject)->page;
            img.id = (uint32_t)(uintptr_t)sp_page->rendererObject;
            premul_alpha = sp_page->pma != 0;
        }
        else if (sp_slot->attachment->type == SP_ATTACHMENT_MESH) {
            spMeshAttachment* mesh = (spMeshAttachment*)sp_slot->attachment;
            att_color = &mesh->color;
            // FIXME(?) early out if the slot alpha is 0
            if (att_color->a == 0) {
                spSkeletonClipping_clipEnd(sp_clip, sp_slot);
                continue;
            }

            const int num_floats = mesh->super.worldVerticesLength;
            num_vertices = num_floats / 2;
            SOKOL_ASSERT(num_vertices <= max_tform_buf_verts);
            spVertexAttachment_computeWorldVertices(&mesh->super, sp_slot, 0, num_floats, tform_buf, 0, tform_buf_stride);
            vertices = tform_buf;
            indices = mesh->triangles;
            num_indices = mesh->trianglesCount; // actually indicesCount???
            uvs = mesh->uvs;
            const spAtlasPage* sp_page = ((spAtlasRegion*)mesh->rendererObject)->page;
            img.id = (uint32_t)(uintptr_t)sp_page->rendererObject;
            premul_alpha = sp_page->pma != 0;
        }
        else if (sp_slot->attachment->type == SP_ATTACHMENT_CLIPPING) {
            spClippingAttachment* clip_attachment = (spClippingAttachment*) sp_slot->attachment;
            spSkeletonClipping_clipStart(sp_clip, sp_slot, clip_attachment);
            continue;
        }
        else {
            spSkeletonClipping_clipEnd(sp_clip, sp_slot);
            continue;
        }
        SOKOL_ASSERT(vertices && (num_vertices > 0));
        SOKOL_ASSERT(indices && (num_indices > 0));
        SOKOL_ASSERT(uvs);
        SOKOL_ASSERT(img.id != SG_INVALID_ID);

        if (spSkeletonClipping_isClipping(sp_clip)) {
            spSkeletonClipping_clipTriangles(sp_clip, tform_buf, num_vertices * 2, (uint16_t*)indices, num_indices, uvs, tform_buf_stride);
            vertices = sp_clip->clippedVertices->items;
            num_vertices = sp_clip->clippedVertices->size / 2;
            uvs = sp_clip->clippedUVs->items;
            indices = sp_clip->clippedTriangles->items;
            num_indices = sp_clip->clippedTriangles->size;
        }
        SOKOL_ASSERT(vertices);
        SOKOL_ASSERT(indices);
        SOKOL_ASSERT(uvs);
        SOKOL_ASSERT(img.id != SG_INVALID_ID);

        // there might be no geometry to render after clipping
        if ((0 == num_vertices) || (0 == num_indices)) {
            spSkeletonClipping_clipEnd(sp_clip, sp_slot);
            continue;
        }

        const _sspine_alloc_vertices_result_t dst_vertices = _sspine_alloc_vertices(ctx, num_vertices);
        const _sspine_alloc_indices_result_t dst_indices = _sspine_alloc_indices(ctx, num_indices);
        if ((0 == dst_vertices.ptr) || (0 == dst_indices.ptr)) {
            spSkeletonClipping_clipEnd(sp_clip, sp_slot);
            continue;
        }

        // write transformed and potentially clipped vertices and indices
        const uint8_t r = (uint8_t)(sp_skel->color.r * sp_slot->color.r * att_color->r * 255.0f);
        const uint8_t g = (uint8_t)(sp_skel->color.g * sp_slot->color.g * att_color->g * 255.0f);
        const uint8_t b = (uint8_t)(sp_skel->color.b * sp_slot->color.b * att_color->b * 255.0f);
        const uint8_t a = (uint8_t)(sp_skel->color.a * sp_slot->color.a * att_color->a * 255.0f);
        const uint32_t color = (uint32_t)((a<<24) | (b<<16) | (g<<8) | r);
        for (int i = 0; i < num_vertices; i++) {
            dst_vertices.ptr[i].pos.x = vertices[i*2];
            dst_vertices.ptr[i].pos.y = vertices[i*2 + 1];
            dst_vertices.ptr[i].color = color;
            dst_vertices.ptr[i].uv.x  = uvs[i*2];
            dst_vertices.ptr[i].uv.y  = uvs[i*2 + 1];
        }
        for (int i = 0; i < num_indices; i++) {
            dst_indices.ptr[i] = (uint32_t)indices[i] + (uint32_t)dst_vertices.index;
        }

        sg_pipeline pip = { SG_INVALID_ID };
        // NOTE: pma == 0.0: use color from texture as is
        //       pma == 1.0: multiply texture rgb by texture alpha in fragment shader
        float pma = 0.0f;
        switch (sp_slot->data->blendMode) {
            case SP_BLEND_MODE_NORMAL:
            case SP_BLEND_MODE_ADDITIVE:
            case SP_BLEND_MODE_SCREEN:
                pip = ctx->pip.normal_additive;
                pma = premul_alpha ? 0.0f : 1.0f;   // NOT A BUG
                break;
            case SP_BLEND_MODE_MULTIPLY:
                pip = ctx->pip.multiply; break;
                pma = 0.0f;     // always use texture color as is
                break;
        }

        // write new draw command, or merge with previous draw command
        _sspine_command_t* prev_cmd = _sspine_prev_command(ctx);
        if (prev_cmd && (prev_cmd->layer == layer) && (prev_cmd->pip.id == pip.id) && (prev_cmd->img.id == img.id) && (prev_cmd->pma == pma)) {
            // merge with previous command
            prev_cmd->num_elements += num_indices;
        }
        else {
            // record a new command
            _sspine_command_t* cmd_ptr = _sspine_next_command(ctx);
            cmd_ptr->layer = layer;
            cmd_ptr->pip = pip;
            cmd_ptr->img = img;
            cmd_ptr->pma = pma;
            cmd_ptr->base_element = dst_indices.index;
            cmd_ptr->num_elements = num_indices;
        }
        spSkeletonClipping_clipEnd(sp_clip, sp_slot);
    }
    spSkeletonClipping_clipEnd2(sp_clip);
}

static _sspine_vsparams_t _sspine_compute_vsparams(const sspine_layer_transform* tform) {
    const float left   = -tform->origin.x;
    const float right  = tform->size.x - tform->origin.x;
    const float top    = -tform->origin.y;
    const float bottom = tform->size.y - tform->origin.y;
    const float znear  = -1.0f;
    const float zfar   = 1.0f;

    // compute orthographic projection matrix
    _sspine_vsparams_t p;
    _sspine_clear(&p, sizeof(p));
    p.mvp[0]  = 2.0f / (right - left);
    p.mvp[1]  = 0.0f;
    p.mvp[2]  = 0.0f;
    p.mvp[3]  = 0.0f;
    p.mvp[4]  = 0.0f;
    p.mvp[5]  = 2.0f / (top - bottom);
    p.mvp[6]  = 0.0f;
    p.mvp[7]  = 0.0f;
    p.mvp[8]  = 0.0f;
    p.mvp[9]  = 0.0f;
    p.mvp[10] = -2.0f / (zfar - znear);
    p.mvp[11] = 0.0f;
    p.mvp[12] = -(right + left) / (right - left);
    p.mvp[13] = -(top + bottom) / (top - bottom);
    p.mvp[14] = -(zfar + znear) / (zfar - znear);
    p.mvp[15] = 1.0f;

    return p;
}

static void _sspine_draw_layer(_sspine_context_t* ctx, int layer, const sspine_layer_transform* tform) {
    if ((ctx->vertices.cur > 0) && (ctx->commands.cur > 0)) {
        sg_push_debug_group("sokol-spine");

        if (ctx->update_frame_count != _sspine.frame_count) {
            ctx->update_frame_count = _sspine.frame_count;
            const sg_range vtx_range = { ctx->vertices.ptr, (size_t)ctx->vertices.cur * sizeof(_sspine_vertex_t) };
            sg_update_buffer(ctx->vbuf, &vtx_range);
            const sg_range idx_range = { ctx->indices.ptr, (size_t)ctx->indices.cur * sizeof(uint32_t) };
            sg_update_buffer(ctx->ibuf, &idx_range);
        }

        _sspine_vsparams_t vsparams = _sspine_compute_vsparams(tform);
        const sg_range vsparams_range = { &vsparams, sizeof(vsparams) };
        _sspine_fsparams_t fsparams;
        _sspine_clear(&fsparams, sizeof(fsparams));
        const sg_range fsparams_range = { &fsparams, sizeof(fsparams) };

        uint32_t cur_pip_id = SG_INVALID_ID;
        uint32_t cur_img_id = SG_INVALID_ID;
        float cur_pma = -1.0f;
        for (int i = 0; i < ctx->commands.cur; i++) {
            const _sspine_command_t* cmd = &ctx->commands.ptr[i];
            if ((layer == cmd->layer) && (sg_query_image_state(cmd->img) == SG_RESOURCESTATE_VALID)) {
                if (cur_pip_id != cmd->pip.id) {
                    sg_apply_pipeline(cmd->pip);
                    cur_pip_id = cmd->pip.id;
                    sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, &vsparams_range);
                    cur_img_id = SG_INVALID_ID;
                }
                if (cur_img_id != cmd->img.id) {
                    ctx->bind.fs_images[0] = cmd->img;
                    sg_apply_bindings(&ctx->bind);
                    cur_img_id = cmd->img.id;
                }
                if (cur_pma != cmd->pma) {
                    fsparams.pma = cmd->pma;
                    sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, &fsparams_range);
                    cur_pma = cmd->pma;
                }
                if (cmd->num_elements > 0) {
                    sg_draw(cmd->base_element, cmd->num_elements, 1);
                }
            }
        }
        sg_pop_debug_group();
    }
}

static void _sspine_init_shared(void) {
    sg_shader_desc shd_desc;
    _sspine_clear(&shd_desc, sizeof(shd_desc));
    shd_desc.attrs[0].name = "position";
    shd_desc.attrs[1].name = "texcoord0";
    shd_desc.attrs[2].name = "color0";
    shd_desc.attrs[0].sem_name = "TEXCOORD";
    shd_desc.attrs[0].sem_index = 0;
    shd_desc.attrs[1].sem_name = "TEXCOORD";
    shd_desc.attrs[1].sem_index = 1;
    shd_desc.attrs[2].sem_name = "TEXCOORD";
    shd_desc.attrs[2].sem_index = 2;
    shd_desc.vs.uniform_blocks[0].size = sizeof(_sspine_vsparams_t);
    shd_desc.vs.uniform_blocks[0].layout = SG_UNIFORMLAYOUT_STD140;
    shd_desc.vs.uniform_blocks[0].uniforms[0].name = "vs_params";
    shd_desc.vs.uniform_blocks[0].uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
    shd_desc.vs.uniform_blocks[0].uniforms[0].array_count = 4;
    shd_desc.fs.uniform_blocks[0].size = 16;
    shd_desc.fs.uniform_blocks[0].layout = SG_UNIFORMLAYOUT_STD140;
    shd_desc.fs.uniform_blocks[0].uniforms[0].name = "fs_params";
    shd_desc.fs.uniform_blocks[0].uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
    shd_desc.fs.uniform_blocks[0].uniforms[0].array_count = 1;
    shd_desc.fs.images[0].name = "tex";
    shd_desc.fs.images[0].image_type = SG_IMAGETYPE_2D;
    shd_desc.fs.images[0].sampler_type = SG_SAMPLERTYPE_FLOAT;
    shd_desc.label = "sspine-shader";
    #if defined(SOKOL_GLCORE33)
        shd_desc.vs.source = _sspine_vs_source_glsl330;
        shd_desc.fs.source = _sspine_fs_source_glsl330;
    #elif defined(SOKOL_GLES2) || defined(SOKOL_GLES3)
        shd_desc.vs.source = _sspine_vs_source_glsl100;
        shd_desc.fs.source = _sspine_fs_source_glsl100;
    #elif defined(SOKOL_METAL)
        shd_desc.vs.entry = "main0";
        shd_desc.fs.entry = "main0";
        switch (sg_query_backend()) {
            case SG_BACKEND_METAL_MACOS:
                shd_desc.vs.bytecode = SG_RANGE(_sspine_vs_bytecode_metal_macos);
                shd_desc.fs.bytecode = SG_RANGE(_sspine_fs_bytecode_metal_macos);
                break;
            case SG_BACKEND_METAL_IOS:
                //FIXME
                //shd_desc.vs.bytecode = SG_RANGE(_sspine_vs_bytecode_metal_ios);
                //shd_desc.fs.bytecode = SG_RANGE(_sspine_fs_bytecode_metal_ios);
                break;
            default:
                // FIXME
                //shd_desc.vs.source = _sspine_vs_source_metal_sim;
                //shd_desc.fs.source = _sspine_fs_source_metal_sim;
                break;
        }
    #elif defined(SOKOL_D3D11)
        shd_desc.vs.bytecode = SG_RANGE(_sspine_vs_bytecode_hlsl4);
        shd_desc.fs.bytecode = SG_RANGE(_sspine_fs_bytecode_hlsl4);
    #elif defined(SOKOL_WGPU)
        shd_desc.vs.bytecode = SG_RANGE(_sspine_vs_bytecode_wgpu);
        shd_desc.fs.bytecode = SG_RANGE(_sspine_fs_bytecode_wgpu);
    #else
        shd_desc.vs.source = _sspine_vs_source_dummy;
        shd_desc.fs.source = _sspine_fs_source_dummy;
    #endif
    _sspine.shd = sg_make_shader(&shd_desc);
}

static void _sspine_destroy_shared(void) {
    sg_destroy_shader(_sspine.shd);
}

static spAnimation* _sspine_lookup_anim(_sspine_instance_t* instance, sspine_anim anim) {
    SOKOL_ASSERT(instance && (instance->slot.id == anim.instance.id));
    SOKOL_ASSERT(instance->sp_skel && instance->sp_skel->data);
    SOKOL_ASSERT((anim.index >= 0) && (anim.index < instance->sp_skel->data->animationsCount));
    return instance->sp_skel->data->animations[anim.index];
}

static spBone* _sspine_lookup_bone(sspine_bone bone) {
    _sspine_instance_t* instance = _sspine_lookup_instance(bone.instance.id);
    if (instance && _sspine_instance_deps_valid(instance)) {
        SOKOL_ASSERT(instance->sp_skel);
        SOKOL_ASSERT(instance->sp_skel->bones);
        SOKOL_ASSERT((bone.index >= 0) && (bone.index <= instance->sp_skel->bonesCount));
        return instance->sp_skel->bones[bone.index];
    }
    return 0;
}

//== PUBLIC FUNCTIONS ==========================================================
SOKOL_API_IMPL void sspine_setup(const sspine_desc* desc) {
    SOKOL_ASSERT(desc);
    spBone_setYDown(1);
    _sspine_clear(&_sspine, sizeof(_sspine));
    _sspine.init_cookie = _SSPINE_INIT_COOKIE;
    _sspine.desc = _sspine_desc_defaults(desc);
    _sspine_init_shared();
    _sspine_setup_context_pool(_sspine.desc.context_pool_size);
    _sspine_setup_atlas_pool(_sspine.desc.atlas_pool_size);
    _sspine_setup_skeleton_pool(_sspine.desc.skeleton_pool_size);
    _sspine_setup_instance_pool(_sspine.desc.instance_pool_size);
    const sspine_context_desc ctx_desc = _sspine_as_context_desc(&_sspine.desc);
    _sspine.def_ctx_id = sspine_make_context(&ctx_desc);
    SOKOL_ASSERT(_sspine_is_default_context(_sspine.def_ctx_id));
    sspine_set_context(_sspine.def_ctx_id);
}

SOKOL_API_IMPL void sspine_shutdown(void) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_destroy_all_instances();
    _sspine_destroy_all_skeletons();
    _sspine_destroy_all_atlases();
    _sspine_destroy_all_contexts();
    _sspine_discard_instance_pool();
    _sspine_discard_skeleton_pool();
    _sspine_discard_atlas_pool();
    _sspine_discard_context_pool();
    _sspine_destroy_shared();
    _sspine.init_cookie = 0;
}

SOKOL_API_IMPL sspine_context sspine_make_context(const sspine_context_desc* desc) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    SOKOL_ASSERT(desc);
    const sspine_context_desc desc_def = _sspine_context_desc_defaults(desc);
    sspine_context ctx_id = _sspine_alloc_context();
    _sspine_context_t* ctx = _sspine_lookup_context(ctx_id.id);
    if (ctx) {
        ctx->slot.state = _sspine_init_context(ctx, &desc_def);
        SOKOL_ASSERT((ctx->slot.state == SSPINE_RESOURCESTATE_VALID) || (ctx->slot.state == SSPINE_RESOURCESTATE_FAILED));
        if (ctx->slot.state == SSPINE_RESOURCESTATE_FAILED) {
            _sspine_deinit_context(ctx);
        }
    }
    else {
        ctx->slot.state = SSPINE_RESOURCESTATE_FAILED;
        SOKOL_LOG("sokol_spine.h: context pool exhausted");
    }
    return ctx_id;
}

SOKOL_API_IMPL void sspine_destroy_context(sspine_context ctx_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    if (_sspine_is_default_context(ctx_id)) {
        SOKOL_LOG("sokol_spine.h: cannot destroy default context");
        return;
    }
    _sspine_destroy_context(ctx_id);
    // re-validate the current context pointer (this will return a nullptr
    // if we just destroyed the current context)
    _sspine.cur_ctx = _sspine_lookup_context(_sspine.cur_ctx_id.id);
}

SOKOL_API_IMPL void sspine_set_context(sspine_context ctx_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    if (_sspine_is_default_context(ctx_id)) {
        _sspine.cur_ctx_id = _sspine.def_ctx_id;
    }
    else {
        _sspine.cur_ctx_id = ctx_id;
    }
    // this will return null if the handle isn't valid
    _sspine.cur_ctx = _sspine_lookup_context(_sspine.cur_ctx_id.id);
}

SOKOL_API_IMPL sspine_context sspine_get_context(void) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    return _sspine.cur_ctx_id;
}

SOKOL_API_IMPL sspine_context sspine_default_context(void) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    return _sspine_make_context_handle(0x00010001);
}

SOKOL_API_IMPL void sspine_new_frame(void) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine.frame_count++;
}

SOKOL_API_IMPL void sspine_update_instance(sspine_instance instance_id, float delta_time) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance && _sspine_instance_deps_valid(instance)) {
        SOKOL_ASSERT(instance->sp_skel);
        SOKOL_ASSERT(instance->sp_anim_state);
        spAnimationState_update(instance->sp_anim_state, delta_time);
        spAnimationState_apply(instance->sp_anim_state, instance->sp_skel);
        spSkeleton_updateWorldTransform(instance->sp_skel);
    }
}

SOKOL_API_IMPL void sspine_draw_instance_in_layer(sspine_instance instance_id, int layer) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_context_t* ctx = _sspine.cur_ctx;
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (ctx && instance && _sspine_instance_deps_valid(instance)) {
        _sspine_draw_instance(ctx, instance, layer);
    }
}

SOKOL_API_IMPL void sspine_context_draw_instance_in_layer(sspine_context ctx_id, sspine_instance instance_id, int layer) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_context_t* ctx = _sspine_lookup_context(ctx_id.id);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (ctx && instance && _sspine_instance_deps_valid(instance)) {
        _sspine_draw_instance(ctx, instance, layer);
    }
}

SOKOL_API_IMPL void sspine_draw_layer(int layer, const sspine_layer_transform* tform) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    SOKOL_ASSERT(tform);
    _sspine_context_t* ctx = _sspine.cur_ctx;
    if (ctx) {
        _sspine_draw_layer(ctx, layer, tform);
    }
}

SOKOL_API_IMPL void sspine_context_draw_layer(sspine_context ctx_id, int layer, const sspine_layer_transform* tform) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    SOKOL_ASSERT(tform);
    _sspine_context_t* ctx = _sspine_lookup_context(ctx_id.id);
    if (ctx) {
        _sspine_draw_layer(ctx, layer, tform);
    }
}

SOKOL_API_IMPL sspine_atlas sspine_make_atlas(const sspine_atlas_desc* desc) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    SOKOL_ASSERT(desc);
    const sspine_atlas_desc desc_def = _sspine_atlas_desc_defaults(desc);
    sspine_atlas atlas_id = _sspine_alloc_atlas();
    _sspine_atlas_t* atlas = _sspine_lookup_atlas(atlas_id.id);
    if (atlas) {
        atlas->slot.state = _sspine_init_atlas(atlas, &desc_def);
        SOKOL_ASSERT((atlas->slot.state == SSPINE_RESOURCESTATE_VALID) || (atlas->slot.state == SSPINE_RESOURCESTATE_FAILED));
        if (atlas->slot.state == SSPINE_RESOURCESTATE_FAILED) {
            _sspine_deinit_atlas(atlas);
        }
    }
    else {
        SOKOL_LOG("sokol_spine.h: atlas pool exhausted");
    }
    return atlas_id;
}

SOKOL_API_IMPL void sspine_destroy_atlas(sspine_atlas atlas_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_destroy_atlas(atlas_id);
}

SOKOL_API_IMPL sspine_skeleton sspine_make_skeleton(const sspine_skeleton_desc* desc) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    SOKOL_ASSERT(desc);
    const sspine_skeleton_desc desc_def = _sspine_skeleton_desc_defaults(desc);
    sspine_skeleton skeleton_id = _sspine_alloc_skeleton();
    _sspine_skeleton_t* skeleton = _sspine_lookup_skeleton(skeleton_id.id);
    if (skeleton) {
        skeleton->slot.state = _sspine_init_skeleton(skeleton, &desc_def);
        SOKOL_ASSERT((skeleton->slot.state == SSPINE_RESOURCESTATE_VALID) || (skeleton->slot.state == SSPINE_RESOURCESTATE_FAILED));
        if (skeleton->slot.state == SSPINE_RESOURCESTATE_FAILED) {
            _sspine_deinit_skeleton(skeleton);
        }
    }
    else {
        SOKOL_LOG("sokol_spine.h: skeleton pool exhausted");
    }
    return skeleton_id;
}

SOKOL_API_IMPL void sspine_destroy_skeleton(sspine_skeleton skeleton_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_destroy_skeleton(skeleton_id);
}

SOKOL_API_IMPL sspine_instance sspine_make_instance(const sspine_instance_desc* desc) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    SOKOL_ASSERT(desc);
    const sspine_instance_desc desc_def = _sspine_instance_desc_defaults(desc);
    sspine_instance instance_id = _sspine_alloc_instance();
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance) {
        instance->slot.state = _sspine_init_instance(instance, &desc_def);
        SOKOL_ASSERT((instance->slot.state == SSPINE_RESOURCESTATE_VALID) || (instance->slot.state == SSPINE_RESOURCESTATE_FAILED));
        if (instance->slot.state == SSPINE_RESOURCESTATE_FAILED) {
            _sspine_deinit_instance(instance);
        }
    }
    else {
        SOKOL_LOG("sokol_spine.h: instance pool exhausted");
    }
    return instance_id;
}

SOKOL_API_IMPL void sspine_destroy_instance(sspine_instance instance_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_destroy_instance(instance_id);
}

SOKOL_API_IMPL sspine_resource_state sspine_get_context_resource_state(sspine_context ctx_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    const _sspine_context_t* ctx = _sspine_lookup_context(ctx_id.id);
    if (ctx) {
        return ctx->slot.state;
    }
    else {
        return SSPINE_RESOURCESTATE_INVALID;
    }
}

SOKOL_API_IMPL sspine_resource_state sspine_get_atlas_resource_state(sspine_atlas atlas_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    const _sspine_atlas_t* atlas = _sspine_lookup_atlas(atlas_id.id);
    if (atlas) {
        return atlas->slot.state;
    }
    else {
        return SSPINE_RESOURCESTATE_INVALID;
    }
}

SOKOL_API_IMPL sspine_resource_state sspine_get_skeleton_resource_state(sspine_skeleton skeleton_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    const _sspine_skeleton_t* skeleton = _sspine_lookup_skeleton(skeleton_id.id);
    if (skeleton) {
        return skeleton->slot.state;
    }
    else {
        return SSPINE_RESOURCESTATE_INVALID;
    }
}

SOKOL_API_IMPL sspine_resource_state sspine_get_instance_resource_state(sspine_instance instance_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    const _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance) {
        return instance->slot.state;
    }
    else {
        return SSPINE_RESOURCESTATE_INVALID;
    }
}

SOKOL_API_IMPL bool sspine_context_valid(sspine_context ctx_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    return sspine_get_context_resource_state(ctx_id) == SSPINE_RESOURCESTATE_VALID;
}

SOKOL_API_IMPL bool sspine_atlas_valid(sspine_atlas atlas_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    return sspine_get_atlas_resource_state(atlas_id) == SSPINE_RESOURCESTATE_VALID;
}

SOKOL_API_IMPL bool sspine_skeleton_valid(sspine_skeleton skeleton_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    return sspine_get_skeleton_resource_state(skeleton_id) == SSPINE_RESOURCESTATE_VALID;
}

SOKOL_API_IMPL bool sspine_instance_valid(sspine_instance instance_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    return sspine_get_instance_resource_state(instance_id) == SSPINE_RESOURCESTATE_VALID;
}

SOKOL_API_IMPL int sspine_num_images(sspine_atlas atlas_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_atlas_t* atlas = _sspine_lookup_atlas(atlas_id.id);
    return atlas ? atlas->num_pages : 0;
}

SOKOL_API_IMPL sspine_image_info sspine_get_image_info(sspine_atlas atlas_id, int image_index) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_atlas_t* atlas = _sspine_lookup_atlas(atlas_id.id);
    sspine_image_info img_info;
    _sspine_clear(&img_info, sizeof(img_info));
    _sspine_init_image_info(atlas, image_index, &img_info);
    return img_info;
}

SOKOL_API_IMPL void sspine_set_position(sspine_instance instance_id, sspine_vec2 position) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance && _sspine_instance_deps_valid(instance)) {
        SOKOL_ASSERT(instance->sp_skel);
        instance->sp_skel->x = position.x;
        instance->sp_skel->y = position.y;
    }
}

SOKOL_API_IMPL void sspine_set_scale(sspine_instance instance_id, sspine_vec2 scale) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance) {
        SOKOL_ASSERT(instance->sp_skel);
        instance->sp_skel->scaleX = scale.x;
        instance->sp_skel->scaleY = scale.y;
    }
}

SOKOL_API_IMPL void sspine_set_color(sspine_instance instance_id, sspine_color color) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance) {
        SOKOL_ASSERT(instance->sp_skel);
        instance->sp_skel->color.r = color.r;
        instance->sp_skel->color.g = color.g;
        instance->sp_skel->color.b = color.b;
        instance->sp_skel->color.a = color.a;
    }
}

SOKOL_API_IMPL sspine_vec2 sspine_get_position(sspine_instance instance_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    sspine_vec2 v = { 0.0f, 0.0f };
    if (instance) {
        SOKOL_ASSERT(instance->sp_skel);
        v.x = instance->sp_skel->x;
        v.y = instance->sp_skel->y;
    }
    return v;
}

SOKOL_API_IMPL sspine_vec2 sspine_get_scale(sspine_instance instance_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    sspine_vec2 v = { 0.0f, 0.0f };
    if (instance) {
        SOKOL_ASSERT(instance->sp_skel);
        v.x = instance->sp_skel->scaleX;
        v.y = instance->sp_skel->scaleY;
    }
    return v;
}

SOKOL_API_IMPL  sspine_color sspine_get_color(sspine_instance instance_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    sspine_color c = { 0.0f, 0.0f, 0.0f, 0.0f };
    if (instance) {
        SOKOL_ASSERT(instance->sp_skel);
        c.r = instance->sp_skel->color.r;
        c.g = instance->sp_skel->color.g;
        c.b = instance->sp_skel->color.b;
        c.a = instance->sp_skel->color.a;
    }
    return c;
}

SOKOL_API_IMPL void sspine_clear_animation_tracks(sspine_instance instance_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance && _sspine_instance_deps_valid(instance)) {
        SOKOL_ASSERT(instance->sp_anim_state);
        spAnimationState_clearTracks(instance->sp_anim_state);
    }
}

SOKOL_API_IMPL void sspine_clear_animation_track(sspine_instance instance_id, int track_index) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance && _sspine_instance_deps_valid(instance)) {
        SOKOL_ASSERT(instance->sp_anim_state);
        spAnimationState_clearTrack(instance->sp_anim_state, track_index);
    }
}

SOKOL_API_IMPL void sspine_set_animation(sspine_instance instance_id, int track_index, sspine_anim anim, bool loop) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance && _sspine_instance_deps_valid(instance) && (instance_id.id == anim.instance.id)) {
        SOKOL_ASSERT(instance->sp_anim_state);
        spAnimation* sp_anim = _sspine_lookup_anim(instance, anim);
        SOKOL_ASSERT(sp_anim);
        spAnimationState_setAnimation(instance->sp_anim_state, track_index, sp_anim, loop?1:0);
    }
}

SOKOL_API_IMPL void sspine_set_animation_by_name(sspine_instance instance_id, int track_index, const char* anim_name, bool loop) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    SOKOL_ASSERT(anim_name);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance && _sspine_instance_deps_valid(instance)) {
        SOKOL_ASSERT(instance->sp_anim_state);
        spAnimationState_setAnimationByName(instance->sp_anim_state, track_index, anim_name, loop?1:0);
    }
}

SOKOL_API_IMPL void sspine_add_animation(sspine_instance instance_id, int track_index, sspine_anim anim, bool loop, float delay) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance && _sspine_instance_deps_valid(instance) && (instance_id.id == anim.instance.id)) {
        SOKOL_ASSERT(instance->sp_anim_state);
        spAnimation* sp_anim = _sspine_lookup_anim(instance, anim);
        SOKOL_ASSERT(sp_anim);
        spAnimationState_addAnimation(instance->sp_anim_state, track_index, sp_anim, loop?1:0, delay);
    }
}

SOKOL_API_IMPL void sspine_add_animation_by_name(sspine_instance instance_id, int track_index, const char* anim_name, bool loop, float delay) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    SOKOL_ASSERT(anim_name);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance && _sspine_instance_deps_valid(instance)) {
        SOKOL_ASSERT(instance->sp_anim_state);
        spAnimationState_addAnimationByName(instance->sp_anim_state, track_index, anim_name, loop?1:0, delay);
    }
}

SOKOL_API_IMPL void sspine_set_empty_animation(sspine_instance instance_id, int track_index, float mix_duration) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance && _sspine_instance_deps_valid(instance)) {
        SOKOL_ASSERT(instance->sp_anim_state);
        spAnimationState_setEmptyAnimation(instance->sp_anim_state, track_index, mix_duration);
    }
}

SOKOL_API_IMPL void sspine_add_empty_animation(sspine_instance instance_id, int track_index, float mix_duration, float delay) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance && _sspine_instance_deps_valid(instance)) {
        SOKOL_ASSERT(instance->sp_anim_state);
        spAnimationState_addEmptyAnimation(instance->sp_anim_state, track_index, mix_duration, delay);
    }
}

SOKOL_API_IMPL int sspine_num_atlas_pages(sspine_atlas atlas_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_atlas_t* atlas = _sspine_lookup_atlas(atlas_id.id);
    if (atlas) {
        return atlas->num_pages;
    }
    else {
        return 0;
    }
}

SOKOL_API_IMPL sspine_atlas_page sspine_atlas_page_at(sspine_atlas atlas_id, int index) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    sspine_atlas_page res;
    _sspine_clear(&res, sizeof(res));
    _sspine_atlas_t* atlas = _sspine_lookup_atlas(atlas_id.id);
    if (atlas && (index >= 0) && (index < atlas->num_pages)) {
        res.atlas = atlas_id;
        res.index = index;
    }
    return res;
}

SOKOL_API_IMPL sspine_atlas_page_info sspine_get_atlas_page_info(sspine_atlas_page page_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    sspine_atlas_page_info res;
    _sspine_clear(&res, sizeof(res));
    _sspine_atlas_t* atlas = _sspine_lookup_atlas(page_id.atlas.id);
    if (atlas) {
        const spAtlasPage* sp_page = _sspine_atlas_page_at(atlas, page_id.index);
        if (sp_page) {
            res.atlas = page_id.atlas;
            res.image.id = (uint32_t)(uintptr_t)sp_page->rendererObject;
            res.name = sp_page->name ? sp_page->name : "";
            res.min_filter = _sspine_as_image_filter(sp_page->minFilter);
            res.mag_filter = _sspine_as_image_filter(sp_page->magFilter);
            res.wrap_u = _sspine_as_image_wrap(sp_page->uWrap);
            res.wrap_v = _sspine_as_image_wrap(sp_page->vWrap);
            res.width = sp_page->width;
            res.height = sp_page->height;
            res.premul_alpha = sp_page->pma != 0;
            res.overrides = atlas->overrides;
        }
    }
    return res;
}

SOKOL_API_IMPL sspine_bone sspine_find_bone(sspine_instance instance_id, const char* name) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    SOKOL_ASSERT(name);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    sspine_bone bone;
    _sspine_clear(&bone, sizeof(bone));
    if (instance && _sspine_instance_deps_valid(instance)) {
        SOKOL_ASSERT(instance->sp_skel);
        spBone* sp_bone = spSkeleton_findBone(instance->sp_skel, name);
        if (sp_bone) {
            SOKOL_ASSERT(sp_bone->data);
            bone.instance = instance_id;
            bone.index = sp_bone->data->index;
        }
    }
    return bone;
}

SOKOL_API_IMPL bool sspine_bone_valid(sspine_bone bone) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    return 0 != _sspine_lookup_bone(bone);
}


SOKOL_API_IMPL int sspine_num_bones(sspine_instance instance_id) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance && _sspine_instance_deps_valid(instance)) {
        SOKOL_ASSERT(instance->sp_skel);
        return instance->sp_skel->bonesCount;
    }
    return 0;
}

SOKOL_API_IMPL sspine_bone sspine_bone_at(sspine_instance instance_id, int index) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    sspine_bone res;
    _sspine_clear(&res, sizeof(res));
    _sspine_instance_t* instance = _sspine_lookup_instance(instance_id.id);
    if (instance && _sspine_instance_deps_valid(instance)) {
        SOKOL_ASSERT(instance->sp_skel);
        if ((index >= 0) && (index < instance->sp_skel->bonesCount)) {
            res.instance = instance_id;
            res.index = index;
        }
    }
    return res;
}

SOKOL_API_IMPL sspine_bone_info sspine_get_bone_info(sspine_bone bone) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    sspine_bone_info res;
    _sspine_clear(&res, sizeof(res));
    spBone* sp_bone = _sspine_lookup_bone(bone);
    if (sp_bone) {
        SOKOL_ASSERT(sp_bone->data);
        SOKOL_ASSERT(sp_bone->data->index == bone.index);
        SOKOL_ASSERT(sp_bone->data->name);
        res.name = sp_bone->data->name;
        res.index = sp_bone->data->index;
        res.length = sp_bone->data->length;
        res.pose_tform.position.x = sp_bone->data->x;
        res.pose_tform.position.y = sp_bone->data->y;
        res.pose_tform.rotation = sp_bone->data->rotation;
        res.pose_tform.scale.x = sp_bone->data->scaleX;
        res.pose_tform.scale.y = sp_bone->data->scaleY;
        res.pose_tform.shear.x = sp_bone->data->shearX;
        res.pose_tform.shear.y = sp_bone->data->shearY;
        res.cur_tform.position.x = sp_bone->x;
        res.cur_tform.position.y = sp_bone->y;
        res.cur_tform.rotation = sp_bone->rotation;
        res.cur_tform.scale.x = sp_bone->scaleX;
        res.cur_tform.scale.y = sp_bone->scaleY;
        res.cur_tform.shear.x = sp_bone->shearX;
        res.cur_tform.shear.y = sp_bone->shearY;
        res.color.r = sp_bone->data->color.r;
        res.color.g = sp_bone->data->color.g;
        res.color.b = sp_bone->data->color.b;
        res.color.a = sp_bone->data->color.a;
    }
    return res;
}

SOKOL_API_IMPL void sspine_set_bone_transform(sspine_bone bone, const sspine_bone_transform* transform) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    SOKOL_ASSERT(transform);
    spBone* sp_bone = _sspine_lookup_bone(bone);
    if (sp_bone) {
        sp_bone->x = transform->position.x;
        sp_bone->y = transform->position.y;
        sp_bone->rotation = transform->rotation;
        sp_bone->scaleX = transform->scale.x;
        sp_bone->scaleY = transform->scale.y;
        sp_bone->shearX = transform->shear.x;
        sp_bone->shearY = transform->shear.y;
    }
}

SOKOL_API_IMPL void sspine_set_bone_position(sspine_bone bone, sspine_vec2 position) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    spBone* sp_bone = _sspine_lookup_bone(bone);
    if (sp_bone) {
        sp_bone->x = position.x;
        sp_bone->y = position.y;
    }
}

SOKOL_API_IMPL void sspine_set_bone_rotation(sspine_bone bone, float rotation) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    spBone* sp_bone = _sspine_lookup_bone(bone);
    if (sp_bone) {
        sp_bone->rotation = rotation;
    }
}

SOKOL_API_IMPL void sspine_set_bone_scale(sspine_bone bone, sspine_vec2 scale) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    spBone* sp_bone = _sspine_lookup_bone(bone);
    if (sp_bone) {
        sp_bone->scaleX = scale.x;
        sp_bone->scaleY = scale.y;
    }
}

SOKOL_API_IMPL void sspine_set_bone_shear(sspine_bone bone, sspine_vec2 shear) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    spBone* sp_bone = _sspine_lookup_bone(bone);
    if (sp_bone) {
        sp_bone->shearX = shear.x;
        sp_bone->shearY = shear.y;
    }
}

SOKOL_API_IMPL sspine_bone_transform sspine_get_bone_transform(sspine_bone bone) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    sspine_bone_transform res;
    _sspine_clear(&res, sizeof(res));
    spBone* sp_bone = _sspine_lookup_bone(bone);
    if (sp_bone) {
        res.position.x = sp_bone->x;
        res.position.y = sp_bone->y;
        res.rotation = sp_bone->rotation;
        res.scale.x = sp_bone->scaleX;
        res.scale.y = sp_bone->scaleY;
        res.shear.x = sp_bone->shearX;
        res.shear.y = sp_bone->shearY;
    }
    return res;
}

SOKOL_API_IMPL sspine_vec2 sspine_get_bone_position(sspine_bone bone) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    sspine_vec2 res;
    _sspine_clear(&res, sizeof(res));
    spBone* sp_bone = _sspine_lookup_bone(bone);
    if (sp_bone) {
        res.x = sp_bone->x;
        res.y = sp_bone->y;
    }
    return res;
}

SOKOL_API_IMPL float sspine_get_bone_rotation(sspine_bone bone) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    spBone* sp_bone = _sspine_lookup_bone(bone);
    if (sp_bone) {
        return sp_bone->rotation;
    }
    else {
        return 0.0f;
    }
}

SOKOL_API_IMPL sspine_vec2 sspine_get_bone_scale(sspine_bone bone) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    sspine_vec2 res;
    _sspine_clear(&res, sizeof(res));
    spBone* sp_bone = _sspine_lookup_bone(bone);
    if (sp_bone) {
        res.x = sp_bone->scaleX;
        res.y = sp_bone->scaleY;
    }
    return res;
}

SOKOL_API_IMPL sspine_vec2 sspine_get_bone_shear(sspine_bone bone) {
    SOKOL_ASSERT(_SSPINE_INIT_COOKIE == _sspine.init_cookie);
    sspine_vec2 res;
    _sspine_clear(&res, sizeof(res));
    spBone* sp_bone = _sspine_lookup_bone(bone);
    if (sp_bone) {
        res.x = sp_bone->shearX;
        res.y = sp_bone->shearY;
    }
    return res;
}

#endif // SOKOL_SPINE_IMPL
#endif // SOKOL_SPINE_INCLUDED
