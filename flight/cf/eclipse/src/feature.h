/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _FEATURE2_H
#define _FEATURE2_H

#include <stdint.h>


class Feature
{
private:
	uint32_t enabledFeatures;


    Feature() { enabledFeatures = 0; }
    Feature(const Feature&);



public:
    static Feature* GetInstance()
    {
        static Feature instance;
        return &instance;
    }

	bool featureConfigured(uint32_t mask);
	bool feature(uint32_t mask);
	void featureSet(uint32_t mask);
	void featureClear(uint32_t mask);
	void featureClearAll(void);
	uint32_t featureMask(void);

};

#endif
