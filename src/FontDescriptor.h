#ifndef FONT_DESCRIPTOR_H
#define FONT_DESCRIPTOR_H
#include <napi.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

enum FontWeight
{
  FontWeightUndefined = 0,
  FontWeightThin = 100,
  FontWeightUltraLight = 200,
  FontWeightLight = 300,
  FontWeightNormal = 400,
  FontWeightMedium = 500,
  FontWeightSemiBold = 600,
  FontWeightBold = 700,
  FontWeightUltraBold = 800,
  FontWeightHeavy = 900
};

enum FontWidth
{
  FontWidthUndefined = 0,
  FontWidthUltraCondensed = 1,
  FontWidthExtraCondensed = 2,
  FontWidthCondensed = 3,
  FontWidthSemiCondensed = 4,
  FontWidthNormal = 5,
  FontWidthSemiExpanded = 6,
  FontWidthExpanded = 7,
  FontWidthExtraExpanded = 8,
  FontWidthUltraExpanded = 9
};

struct FontDescriptor
{
public:
  const char *path;
  const char *postscriptName;
  const char *family;
  const char *style;
  FontWeight weight;
  FontWidth width;
  bool italic;
  bool monospace;

  FontDescriptor(Napi::Env env, Napi::Object obj)
  {
    path = NULL;
    postscriptName = getString(env, obj, "postscriptName");
    family = getString(env, obj, "family");
    style = getString(env, obj, "style");
    weight = (FontWeight)getNumber(env, obj, "weight");
    width = (FontWidth)getNumber(env, obj, "width");
    italic = getBool(env, obj, "italic");
    monospace = getBool(env, obj, "monospace");
  }

  FontDescriptor()
  {
    path = NULL;
    postscriptName = NULL;
    family = NULL;
    style = NULL;
    weight = FontWeightUndefined;
    width = FontWidthUndefined;
    italic = false;
    monospace = false;
  }

  FontDescriptor(const char *path, const char *postscriptName, const char *family, const char *style,
                 FontWeight weight, FontWidth width, bool italic, bool monospace)
  {
    this->path = copyString(path);
    this->postscriptName = copyString(postscriptName);
    this->family = copyString(family);
    this->style = copyString(style);
    this->weight = weight;
    this->width = width;
    this->italic = italic;
    this->monospace = monospace;
  }

  FontDescriptor(FontDescriptor *desc)
  {
    path = copyString(desc->path);
    postscriptName = copyString(desc->postscriptName);
    family = copyString(desc->family);
    style = copyString(desc->style);
    weight = desc->weight;
    width = desc->width;
    italic = desc->italic;
    monospace = desc->monospace;
  }

  ~FontDescriptor()
  {
    if (path)
      delete path;

    if (postscriptName)
      delete postscriptName;

    if (family)
      delete family;

    if (style)
      delete style;

    postscriptName = NULL;
    family = NULL;
    style = NULL;
  }

  Napi::Object toJSObject(Napi::Env env)
  {
    Napi::Object res = Napi::Object::New(env);

    if (path)
    {
      res.Set(Napi::String::New(env, "path"), Napi::String::New(env, path));
    }

    if (postscriptName)
    {
      res.Set(Napi::String::New(env, "postscriptName"), Napi::String::New(env, postscriptName));
    }

    if (family)
    {
      res.Set(Napi::String::New(env, "family"), Napi::String::New(env, family));
    }

    if (style)
    {
      res.Set(Napi::String::New(env, "style"), Napi::String::New(env, style));
    }

    res.Set(Napi::String::New(env, "weight"), Napi::Number::New(env, weight));
    res.Set(Napi::String::New(env, "width"), Napi::Number::New(env, width));
    res.Set(Napi::String::New(env, "italic"), Napi::Boolean::New(env, italic));
    res.Set(Napi::String::New(env, "monospace"), Napi::Boolean::New(env, monospace));
    return res;
  }

private:
  char *copyString(const char *input)
  {
    if (!input)
      return NULL;

    char *str = new char[strlen(input) + 1];
    strcpy(str, input);
    return str;
  }

  char *getString(Napi::Env env, Napi::Object obj, const char *name)
  {
    Napi::Value value = obj.Get(Napi::String::New(env, name));

    if (!value.IsEmpty() && value.IsString())
    {
      return copyString(value.As<Napi::String>().Utf8Value().c_str());
    }

    return NULL;
  }

  int getNumber(Napi::Env env, Napi::Object obj, const char *name)
  {
    Napi::Value value = obj.Get(Napi::String::New(env, name));

    if (!value.IsEmpty() && value.IsNumber())
    {
      return value.As<Napi::Number>().Int32Value();
    }

    return 0;
  }

  bool getBool(Napi::Env env, Napi::Object obj, const char *name)
  {
    Napi::Value value = obj.Get(Napi::String::New(env, name));

    if (!value.IsEmpty() && value.IsBoolean())
    {
      return value.As<Napi::Boolean>().Value();
    }

    return false;
  }
};

class ResultSet : public std::vector<FontDescriptor *>
{
public:
  ~ResultSet()
  {
    for (ResultSet::iterator it = this->begin(); it != this->end(); it++)
    {
      delete *it;
    }
  }
};

#endif
