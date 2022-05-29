#pragma once

#include "GFxObject.h"

class GFxDisplayObject : public GFxObject
{
public:

	GFxDisplayObject() = default;

	GFxDisplayObject(RE::GFxMovieView* a_movieView, const std::string_view& a_pathToMovieClip)
	: GFxObject{ a_movieView, a_pathToMovieClip }
	{
		assert(IsDisplayObject());
	}

	GFxDisplayObject(const RE::GFxValue& a_value)
	: GFxObject{ a_value }
	{
		assert(a_value.IsDisplayObject());
	}

	bool operator==(const RE::GFxValue& a_rhs) const
	{
		if (a_rhs.IsDisplayObject()) 
		{
			auto& other = reinterpret_cast<const GFxDisplayObject&>(a_rhs);

			return _value.obj == other._value.obj;
		}

		return false;
	}

	GFxDisplayObject CreateEmptyMovieClip(const std::string_view& a_name, std::int32_t a_depth)
	{
		GFxDisplayObject mc;
		RE::GFxValue::CreateEmptyMovieClip(&mc, a_name.data(), a_depth);
		return mc;
	}

	GFxDisplayObject CreateEmptyMovieClip(const std::string_view& a_name)
	{
		return CreateEmptyMovieClip(a_name, GetNextHighestDepth());
	}

	GFxDisplayObject AttachMovie(const std::string_view& a_className, const std::string_view& a_name, std::int32_t a_depth)
	{
		GFxDisplayObject mc;
		RE::GFxValue::AttachMovie(&mc, a_className.data(), a_name.data(), a_depth);
		mc.GetMovieView()->Advance(0.0F);
		return mc;
	}

	GFxDisplayObject AttachMovie(const std::string_view& a_className, const std::string_view& a_name)
	{
		return AttachMovie(a_className, a_name, GetNextHighestDepth());
	}

	std::int32_t GetNextHighestDepth()
	{
		return static_cast<std::int32_t>(Invoke("getNextHighestDepth").GetNumber());
	}

	void SwapDepths(std::int32_t a_depth)
	{
		Invoke("swapDepths", a_depth);
	}

	void LoadMovie(const std::string_view& a_swfPath)
	{
		Invoke("loadMovie", a_swfPath.data());
	}

	void UnloadMovie()
	{
		Invoke("unloadMovie");
	}

	void RemoveMovieClip()
	{
		// MovieClip.removeMovieClip() does not remove a movie clip assigned
		// to a negative depth value. Movie clips created in the authoring tool
		// are assigned negative depth values by default. To remove a movie clip
		// that is assigned to a negative depth value, first use the MovieClip.swapDepths()
		// method to move the movie clip to a positive depth value.
		// Reference: http://homepage.divms.uiowa.edu/~slonnegr/flash/ActionScript2Reference.pdf#page=917
		SwapDepths(1);
		Invoke("removeMovieClip");
	}

	RE::GPointF LocalToGlobal()
	{
		GFxObject pt;
		GetMovieView()->CreateObject(&pt);

		assert(pt.IsObject());

		RE::GFxValue x = 0.0;
		pt.SetMember("x", x);

		RE::GFxValue y = 0.0;
		pt.SetMember("y", y);

		Invoke("localToGlobal", pt);

		x = pt.GetMember("x");
		y = pt.GetMember("y");

		return { static_cast<float>(x.GetNumber()), static_cast<float>(y.GetNumber()) };
	}
};