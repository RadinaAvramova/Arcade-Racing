/*
The MIT License (MIT)

Copyright (c) 2015 Tayfun Kayhan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "GODLoader.h"

#include <OgreDataStream.h>
#include <OgreResourceGroupManager.h>
#include <OgreLog.h>

#include "Game.h"
#include "Utils.h"

using namespace rapidxml;

GODLoader::GODLoader()
	: m_logger(NULL), m_xmlDoc(NULL), m_xmlContent(NULL)
{
	m_logger = Game::getSingleton().getLog();

	Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource("defs.god", "General");
	m_xmlContent = strdup(stream->getAsString().c_str());
	m_xmlDoc = new xml_document<> ();
	m_xmlDoc->parse<0>(m_xmlContent);

	loadAllDefinitions();
}

GODLoader::~GODLoader()
{
	m_xmlDoc->clear();
	SAFE_DELETE(m_xmlContent);
	SAFE_DELETE(m_xmlDoc);

	for (auto& it = m_loadedDefinitions.begin(); it != m_loadedDefinitions.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
	m_loadedDefinitions.clear();

	m_logger->logMessage("GODs destroyed");
	m_logger->logMessage("GODLoader destroyed");
}

GODLoader& GODLoader::getSingleton()
{
	static GODLoader* onlyInstance = new GODLoader();

	return *onlyInstance;
}

GOD* GODLoader::getGameObjectDefinitionByTag(const Ogre::String& tag)
{
	GOD* def = NULL;
	const auto it = m_loadedDefinitions.find(tag);
	if (it == m_loadedDefinitions.end())
		def = m_loadedDefinitions["default_static"];
	else
		def = (*it).second;

	return def;
}

Ogre::String GODLoader::getGameObjectDefinitionValue(const Ogre::String& tag, const Ogre::String& key) const
{
	Ogre::String val;
	const auto it = m_loadedDefinitions.find(tag);
	if (it != m_loadedDefinitions.end())
	{
		val = it->second->m_params[key];
	}

	return val;
}

bool GODLoader::loadAllDefinitions()
{
	xml_node<>* headNode = m_xmlDoc->first_node("game_object_definition")->first_node();
	for (; headNode; headNode = headNode->next_sibling())
	{
		xml_node<>* attsNode = headNode->first_node();
		Ogre::String tag = headNode->first_attribute("name")->value();

		GOD* def = new GOD();
		for (; attsNode; attsNode = attsNode->next_sibling())
		{
			Ogre::String name = attsNode->name();
			Ogre::String value = attsNode->first_attribute("value")->value();

			def->m_params.insert(std::make_pair(name, value));
		}

		m_logger->logMessage("Inserting GOD" + tag);
		m_loadedDefinitions.insert(std::make_pair(tag, def));
	}

	m_logger->logMessage("All GODs loaded");
	m_logger->logMessage("game_scene.pod parsed");

	return true;
}