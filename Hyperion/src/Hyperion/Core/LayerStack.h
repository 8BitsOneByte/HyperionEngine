//
// Created by 13191 on 2026/1/3.
//

#ifndef HYPERIONENGINE_LAYERSTACK_H
#define HYPERIONENGINE_LAYERSTACK_H

#include "Layer.h"
#include "Hyperion/Core/Core.h"

#include <vector>

namespace Hyperion
{
    class HYPERION_API LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }

    private:
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    };
} // Hyperion

#endif //HYPERIONENGINE_LAYERSTACK_H