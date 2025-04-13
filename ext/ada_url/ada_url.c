#include "ada_url.h"

typedef struct {
  ada_url url;
} ada_url_wrapper;

static ada_url_wrapper* get_url_wrapper(VALUE self) {
  return (ada_url_wrapper*)DATA_PTR(self);
}

static VALUE ada_url_free(void* ptr) {
  ada_url_wrapper* wrapper = (ada_url_wrapper*)ptr;
  if (wrapper) {
    if (wrapper->url) {
      ada_free(wrapper->url);
      wrapper->url = NULL;
    }
    free(wrapper);
  }
  return Qnil;
}

static VALUE ada_url_alloc(VALUE klass) {
  ada_url_wrapper* wrapper = malloc(sizeof(ada_url_wrapper));
  if (!wrapper) {
    rb_raise(rb_eNoMemError, "Failed to allocate memory for AdaUrl instance");
  }
  wrapper->url = NULL;
  return Data_Wrap_Struct(klass, NULL, ada_url_free, wrapper);
}

static VALUE ada_url_initialize(VALUE self, VALUE input) {
  const char* input_str = StringValueCStr(input);
  size_t length = RSTRING_LEN(input);

  ada_url url = ada_parse(input_str, length);
  if (!url) {
    rb_raise(rb_eArgError, "Invalid URL");
  }

  ada_url_wrapper* wrapper = malloc(sizeof(ada_url_wrapper));
  wrapper->url = url;
  DATA_PTR(self) = wrapper;

  return self;
}

#define DEFINE_ADA_URL_GETTER(PROPERTY) \
static VALUE ada_url_##PROPERTY(VALUE self) { \
  ada_url_wrapper* wrapper = get_url_wrapper(self); \
  ada_string PROPERTY = ada_get_##PROPERTY(wrapper->url); \
  return rb_str_new(PROPERTY.data, PROPERTY.length); \
}

DEFINE_ADA_URL_GETTER(href);
DEFINE_ADA_URL_GETTER(username);
DEFINE_ADA_URL_GETTER(password);
DEFINE_ADA_URL_GETTER(port);
DEFINE_ADA_URL_GETTER(hash);
DEFINE_ADA_URL_GETTER(host);
DEFINE_ADA_URL_GETTER(hostname);
DEFINE_ADA_URL_GETTER(pathname);
DEFINE_ADA_URL_GETTER(search);
DEFINE_ADA_URL_GETTER(protocol);

static VALUE ada_rb_is_valid(VALUE self) {
  ada_url_wrapper* wrapper = get_url_wrapper(self);
  return ada_is_valid(wrapper->url) ? Qtrue : Qfalse;
}

static VALUE rb_mAdaUrl_symbol_common_host;
static VALUE rb_mAdaUrl_symbol_ipv4;
static VALUE rb_mAdaUrl_symbol_ipv6;

static VALUE ada_rb_get_host_type(VALUE self) {
  ada_url_wrapper* wrapper = get_url_wrapper(self);
  switch (ada_get_host_type(wrapper->url)) {
    case 0: return rb_mAdaUrl_symbol_common_host;
    case 1: return rb_mAdaUrl_symbol_ipv4;
    case 2: return rb_mAdaUrl_symbol_ipv6;
  }
  rb_raise(rb_eTypeError, "Fell through the switch... is there a new host type?");
  return Qnil;
}

VALUE rb_mAdaUrl = Qnil;
VALUE cAdaUrl = Qnil;

RUBY_FUNC_EXPORTED void Init_ada_url(void)
{
  rb_mAdaUrl = rb_define_module("AdaUrl");
  cAdaUrl = rb_define_class_under(rb_mAdaUrl, "Url", rb_cObject);

  // Symbols
  {
    rb_mAdaUrl_symbol_common_host = ID2SYM(rb_intern("common"));
    rb_mAdaUrl_symbol_ipv4 = ID2SYM(rb_intern("ipv4"));
    rb_mAdaUrl_symbol_ipv6 = ID2SYM(rb_intern("ipv6"));
  }

  rb_define_alloc_func(cAdaUrl, ada_url_alloc);

  rb_define_method(cAdaUrl, "initialize", ada_url_initialize, 1);

  // Getter methods
  {
    #define ATTACH_GETTER(PROPERTY) \
      rb_define_method(cAdaUrl, #PROPERTY, ada_url_##PROPERTY, 0);

    ATTACH_GETTER(href);
    ATTACH_GETTER(username);
    ATTACH_GETTER(password);
    ATTACH_GETTER(port);
    ATTACH_GETTER(hash);
    ATTACH_GETTER(host);
    ATTACH_GETTER(hostname);
    ATTACH_GETTER(pathname);
    ATTACH_GETTER(search);
    ATTACH_GETTER(protocol);

    rb_define_method(cAdaUrl, "host_type", ada_rb_get_host_type, 0);

    rb_define_method(cAdaUrl, "valid?", ada_rb_is_valid, 0);
  }
}
