/**
 * @file ST_Bindable.h
 * @author Lhxl
 * @date 2025-2-12
 * @version build15
 */

#ifndef ST_BINDABLE_H
#define ST_BINDABLE_H

#include "Bindable.h"

class ST_Bindable : public Bindable {
	void Bind(Graphics& gfx) noexcept override;
};

#endif