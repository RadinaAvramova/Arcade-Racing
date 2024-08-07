
#pragma once

#include <map>
#include <vector>

#include <OgreString.h>

#include <OgreRenderQueue.h>

#define SAFE_DELETE(x) { if (x) { delete x; x = NULL; } }
#define SAFE_DELETE_ARRAY(x) { if (x) delete[] x; x = NULL;}

#define BIT_ME(x) (1 << x)

#if _DEBUG
#define LOG_ME(msg) { printf("%s\n", msg); }
#else
#define LOG_ME(msg) //printf("");
#endif

struct PlayerStats;

class Utils
{
public:
	static bool doesIncludeStr(const Ogre::String& source, const Ogre::String& check, bool lower = false);

	static const Ogre::uint32 RV_Hud = 2;

	static Ogre::String stringifyStatsTiming(PlayerStats* stats);

	static const Ogre::uint8 RQG_Hud1 = Ogre::RENDER_QUEUE_OVERLAY - 5;
	static const Ogre::uint8 RQG_Hud2 = Ogre::RENDER_QUEUE_OVERLAY - 2;
	static const Ogre::uint8 RQG_Hud3 = Ogre::RENDER_QUEUE_OVERLAY - 1;

	/* Creates a Comparison Functor for STL containers having Key* objects that have operator< overloaded */
	template<class T>
	struct ComparisonFunctor
	{
		bool operator()(T* const lhs, T* const rhs) const
		{
			return (*lhs < *rhs);
		}
	};

private:
	Utils();
	~Utils();
};
