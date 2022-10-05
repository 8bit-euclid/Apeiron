/***************************************************************************************************************************************************************
* GPL-3.0 License
* Copyright (C) 2022 Niran A. Ilangakoon
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************************************************************************/

#pragma once

#include "../../../include/Global.h"

namespace aprn::vis {

template<class T>
Scene&
Scene::Add(T&& object)
{
   AddObject(std::forward<T>(object));
   return *this;
}

template<class T>
void
Scene::AddObject(const T& object)
{
   if      constexpr(isTypeSame<T, Model>())            Actors_.push_back(std::make_shared<Model>(object));
   else if constexpr(isTypeSame<T, ModelGroup>())       Actors_.push_back(std::make_shared<ModelGroup>(object));
   else if constexpr(isTypeSame<T, DirectionalLight>()) DLights_.push_back(object);
   else if constexpr(isTypeSame<T, PointLight>())       PLights_.push_back(object);
   else if constexpr(isTypeSame<T, SpotLight>())        SLights_.push_back(object);
   else if constexpr(isTypeSame<T, TeXBox>())
   {
      TeXBoxes_.push_back(std::make_shared<TeXBox>(object));
      Actors_.push_back(TeXBoxes_.back());
   }
}

template<class T>
void
Scene::AddObject(T&& object)
{
   if      constexpr(isTypeSame<T, Model>())            Actors_.push_back(std::make_shared<Model>(std::move(object)));
   else if constexpr(isTypeSame<T, ModelGroup>())       Actors_.push_back(std::make_shared<ModelGroup>(std::move(object)));
   else if constexpr(isTypeSame<T, DirectionalLight>()) DLights_.push_back(std::move(object));
   else if constexpr(isTypeSame<T, PointLight>())       PLights_.push_back(std::move(object));
   else if constexpr(isTypeSame<T, SpotLight>())        SLights_.push_back(std::move(object));
   else if constexpr(isTypeSame<T, TeXBox>())
   {
      TeXBoxes_.push_back(std::make_shared<TeXBox>(std::move(object)));
      Actors_.push_back(TeXBoxes_.back());
   }
}

}