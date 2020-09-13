#ifndef LAYER_STACK_H
#define LAYER_STACK_H

/* Uciniti engine includes
*/
#include "core.h"
#include "layer.h"

/* Standard library includes
*/
#include <vector>

namespace Uciniti
{
	class layer_stack
	{
	public:
		layer_stack();
		~layer_stack();

		void push_layer(layer* a_layer);
		void push_overlay(layer* a_overlay);
		void pop_layer(layer* a_layer);
		void pop_overlay(layer* a_overlay);

		std::vector<layer*>::iterator begin() { return layers.begin(); }
		std::vector<layer*>::iterator end() { return layers.end(); }

	private:
		std::vector<layer*> layers;
		std::vector<layer*>::iterator layer_insert;
	};
}

#endif // !LAYER_STACK_H