/* Pre-comp header include
*/
#include "uvkpch.h"

/* Class header include
*/
#include "layer_stack.h"

namespace Uciniti
{
	layer_stack::layer_stack()
	{
		layer_insert = layers.begin();
	}

	layer_stack::~layer_stack()
	{
		for (layer* this_layer : layers)
		{
			delete this_layer;
			this_layer = nullptr;
		}
	}

	void layer_stack::push_layer(layer* a_layer)
	{
		layer_insert = layers.emplace(layer_insert, a_layer);
	}

	void layer_stack::push_overlay(layer* a_overlay)
	{
		layers.emplace_back(a_overlay);
	}

	void layer_stack::pop_layer(layer* a_layer)
	{
		auto it = std::find(layers.begin(), layers.end(), a_layer);
		if (it != layers.end())
		{
			layers.erase(it);
			layer_insert--;
		}
	}

	void layer_stack::pop_overlay(layer* a_overlay)
	{
		auto it = std::find(layers.begin(), layers.end(), a_overlay);
		if (it != layers.end())
			layers.erase(it);
	}
}