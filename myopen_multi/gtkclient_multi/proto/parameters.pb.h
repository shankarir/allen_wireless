// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: parameters.proto

#ifndef PROTOBUF_parameters_2eproto__INCLUDED
#define PROTOBUF_parameters_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
// @@protoc_insertion_point(includes)

namespace Configuration {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_parameters_2eproto();
void protobuf_AssignDesc_parameters_2eproto();
void protobuf_ShutdownFile_parameters_2eproto();

class units;
class channels;
class radios;
class parameters;
class state;

// ===================================================================

class units : public ::google::protobuf::Message {
 public:
  units();
  virtual ~units();
  
  units(const units& from);
  
  inline units& operator=(const units& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const units& default_instance();
  
  void Swap(units* other);
  
  // implements Message ----------------------------------------------
  
  units* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const units& from);
  void MergeFrom(const units& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required float aperture = 1;
  inline bool has_aperture() const;
  inline void clear_aperture();
  static const int kApertureFieldNumber = 1;
  inline float aperture() const;
  inline void set_aperture(float value);
  
  // required float pca_scl = 2;
  inline bool has_pca_scl() const;
  inline void clear_pca_scl();
  static const int kPcaSclFieldNumber = 2;
  inline float pca_scl() const;
  inline void set_pca_scl(float value);
  
  // repeated float pca = 3 [packed = true];
  inline int pca_size() const;
  inline void clear_pca();
  static const int kPcaFieldNumber = 3;
  inline float pca(int index) const;
  inline void set_pca(int index, float value);
  inline void add_pca(float value);
  inline const ::google::protobuf::RepeatedField< float >&
      pca() const;
  inline ::google::protobuf::RepeatedField< float >*
      mutable_pca();
  
  // repeated float templates = 4 [packed = true];
  inline int templates_size() const;
  inline void clear_templates();
  static const int kTemplatesFieldNumber = 4;
  inline float templates(int index) const;
  inline void set_templates(int index, float value);
  inline void add_templates(float value);
  inline const ::google::protobuf::RepeatedField< float >&
      templates() const;
  inline ::google::protobuf::RepeatedField< float >*
      mutable_templates();
  
  // @@protoc_insertion_point(class_scope:Configuration.units)
 private:
  inline void set_has_aperture();
  inline void clear_has_aperture();
  inline void set_has_pca_scl();
  inline void clear_has_pca_scl();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  float aperture_;
  float pca_scl_;
  ::google::protobuf::RepeatedField< float > pca_;
  mutable int _pca_cached_byte_size_;
  ::google::protobuf::RepeatedField< float > templates_;
  mutable int _templates_cached_byte_size_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_parameters_2eproto();
  friend void protobuf_AssignDesc_parameters_2eproto();
  friend void protobuf_ShutdownFile_parameters_2eproto();
  
  void InitAsDefaultInstance();
  static units* default_instance_;
};
// -------------------------------------------------------------------

class channels : public ::google::protobuf::Message {
 public:
  channels();
  virtual ~channels();
  
  channels(const channels& from);
  
  inline channels& operator=(const channels& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const channels& default_instance();
  
  void Swap(channels* other);
  
  // implements Message ----------------------------------------------
  
  channels* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const channels& from);
  void MergeFrom(const channels& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);
  
  // required int32 ch = 2;
  inline bool has_ch() const;
  inline void clear_ch();
  static const int kChFieldNumber = 2;
  inline ::google::protobuf::int32 ch() const;
  inline void set_ch(::google::protobuf::int32 value);
  
  // required float gain = 3;
  inline bool has_gain() const;
  inline void clear_gain();
  static const int kGainFieldNumber = 3;
  inline float gain() const;
  inline void set_gain(float value);
  
  // required float agc = 4;
  inline bool has_agc() const;
  inline void clear_agc();
  static const int kAgcFieldNumber = 4;
  inline float agc() const;
  inline void set_agc(float value);
  
  // required float centering = 5;
  inline bool has_centering() const;
  inline void clear_centering();
  static const int kCenteringFieldNumber = 5;
  inline float centering() const;
  inline void set_centering(float value);
  
  // required float threshold = 6;
  inline bool has_threshold() const;
  inline void clear_threshold();
  static const int kThresholdFieldNumber = 6;
  inline float threshold() const;
  inline void set_threshold(float value);
  
  // repeated .Configuration.units unit = 7;
  inline int unit_size() const;
  inline void clear_unit();
  static const int kUnitFieldNumber = 7;
  inline const ::Configuration::units& unit(int index) const;
  inline ::Configuration::units* mutable_unit(int index);
  inline ::Configuration::units* add_unit();
  inline const ::google::protobuf::RepeatedPtrField< ::Configuration::units >&
      unit() const;
  inline ::google::protobuf::RepeatedPtrField< ::Configuration::units >*
      mutable_unit();
  
  // repeated float pca_mean = 8 [packed = true];
  inline int pca_mean_size() const;
  inline void clear_pca_mean();
  static const int kPcaMeanFieldNumber = 8;
  inline float pca_mean(int index) const;
  inline void set_pca_mean(int index, float value);
  inline void add_pca_mean(float value);
  inline const ::google::protobuf::RepeatedField< float >&
      pca_mean() const;
  inline ::google::protobuf::RepeatedField< float >*
      mutable_pca_mean();
  
  // repeated float pca_max = 9 [packed = true];
  inline int pca_max_size() const;
  inline void clear_pca_max();
  static const int kPcaMaxFieldNumber = 9;
  inline float pca_max(int index) const;
  inline void set_pca_max(int index, float value);
  inline void add_pca_max(float value);
  inline const ::google::protobuf::RepeatedField< float >&
      pca_max() const;
  inline ::google::protobuf::RepeatedField< float >*
      mutable_pca_max();
  
  // @@protoc_insertion_point(class_scope:Configuration.channels)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_ch();
  inline void clear_has_ch();
  inline void set_has_gain();
  inline void clear_has_gain();
  inline void set_has_agc();
  inline void clear_has_agc();
  inline void set_has_centering();
  inline void clear_has_centering();
  inline void set_has_threshold();
  inline void clear_has_threshold();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::int32 id_;
  ::google::protobuf::int32 ch_;
  float gain_;
  float agc_;
  float centering_;
  float threshold_;
  ::google::protobuf::RepeatedPtrField< ::Configuration::units > unit_;
  ::google::protobuf::RepeatedField< float > pca_mean_;
  mutable int _pca_mean_cached_byte_size_;
  ::google::protobuf::RepeatedField< float > pca_max_;
  mutable int _pca_max_cached_byte_size_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(9 + 31) / 32];
  
  friend void  protobuf_AddDesc_parameters_2eproto();
  friend void protobuf_AssignDesc_parameters_2eproto();
  friend void protobuf_ShutdownFile_parameters_2eproto();
  
  void InitAsDefaultInstance();
  static channels* default_instance_;
};
// -------------------------------------------------------------------

class radios : public ::google::protobuf::Message {
 public:
  radios();
  virtual ~radios();
  
  radios(const radios& from);
  
  inline radios& operator=(const radios& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const radios& default_instance();
  
  void Swap(radios* other);
  
  // implements Message ----------------------------------------------
  
  radios* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const radios& from);
  void MergeFrom(const radios& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::uint32 id() const;
  inline void set_id(::google::protobuf::uint32 value);
  
  // repeated .Configuration.channels channel = 2;
  inline int channel_size() const;
  inline void clear_channel();
  static const int kChannelFieldNumber = 2;
  inline const ::Configuration::channels& channel(int index) const;
  inline ::Configuration::channels* mutable_channel(int index);
  inline ::Configuration::channels* add_channel();
  inline const ::google::protobuf::RepeatedPtrField< ::Configuration::channels >&
      channel() const;
  inline ::google::protobuf::RepeatedPtrField< ::Configuration::channels >*
      mutable_channel();
  
  // @@protoc_insertion_point(class_scope:Configuration.radios)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::RepeatedPtrField< ::Configuration::channels > channel_;
  ::google::protobuf::uint32 id_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_parameters_2eproto();
  friend void protobuf_AssignDesc_parameters_2eproto();
  friend void protobuf_ShutdownFile_parameters_2eproto();
  
  void InitAsDefaultInstance();
  static radios* default_instance_;
};
// -------------------------------------------------------------------

class parameters : public ::google::protobuf::Message {
 public:
  parameters();
  virtual ~parameters();
  
  parameters(const parameters& from);
  
  inline parameters& operator=(const parameters& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const parameters& default_instance();
  
  void Swap(parameters* other);
  
  // implements Message ----------------------------------------------
  
  parameters* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const parameters& from);
  void MergeFrom(const parameters& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required int32 signal_chain = 1;
  inline bool has_signal_chain() const;
  inline void clear_signal_chain();
  static const int kSignalChainFieldNumber = 1;
  inline ::google::protobuf::int32 signal_chain() const;
  inline void set_signal_chain(::google::protobuf::int32 value);
  
  // repeated .Configuration.radios radio = 2;
  inline int radio_size() const;
  inline void clear_radio();
  static const int kRadioFieldNumber = 2;
  inline const ::Configuration::radios& radio(int index) const;
  inline ::Configuration::radios* mutable_radio(int index);
  inline ::Configuration::radios* add_radio();
  inline const ::google::protobuf::RepeatedPtrField< ::Configuration::radios >&
      radio() const;
  inline ::google::protobuf::RepeatedPtrField< ::Configuration::radios >*
      mutable_radio();
  
  // repeated int32 selected = 3;
  inline int selected_size() const;
  inline void clear_selected();
  static const int kSelectedFieldNumber = 3;
  inline ::google::protobuf::int32 selected(int index) const;
  inline void set_selected(int index, ::google::protobuf::int32 value);
  inline void add_selected(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      selected() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_selected();
  
  // @@protoc_insertion_point(class_scope:Configuration.parameters)
 private:
  inline void set_has_signal_chain();
  inline void clear_has_signal_chain();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::RepeatedPtrField< ::Configuration::radios > radio_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > selected_;
  ::google::protobuf::int32 signal_chain_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  friend void  protobuf_AddDesc_parameters_2eproto();
  friend void protobuf_AssignDesc_parameters_2eproto();
  friend void protobuf_ShutdownFile_parameters_2eproto();
  
  void InitAsDefaultInstance();
  static parameters* default_instance_;
};
// -------------------------------------------------------------------

class state : public ::google::protobuf::Message {
 public:
  state();
  virtual ~state();
  
  state(const state& from);
  
  inline state& operator=(const state& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const state& default_instance();
  
  void Swap(state* other);
  
  // implements Message ----------------------------------------------
  
  state* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const state& from);
  void MergeFrom(const state& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional string default_configuration = 1;
  inline bool has_default_configuration() const;
  inline void clear_default_configuration();
  static const int kDefaultConfigurationFieldNumber = 1;
  inline const ::std::string& default_configuration() const;
  inline void set_default_configuration(const ::std::string& value);
  inline void set_default_configuration(const char* value);
  inline void set_default_configuration(const char* value, size_t size);
  inline ::std::string* mutable_default_configuration();
  inline ::std::string* release_default_configuration();
  
  // @@protoc_insertion_point(class_scope:Configuration.state)
 private:
  inline void set_has_default_configuration();
  inline void clear_has_default_configuration();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* default_configuration_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_parameters_2eproto();
  friend void protobuf_AssignDesc_parameters_2eproto();
  friend void protobuf_ShutdownFile_parameters_2eproto();
  
  void InitAsDefaultInstance();
  static state* default_instance_;
};
// ===================================================================


// ===================================================================

// units

// required float aperture = 1;
inline bool units::has_aperture() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void units::set_has_aperture() {
  _has_bits_[0] |= 0x00000001u;
}
inline void units::clear_has_aperture() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void units::clear_aperture() {
  aperture_ = 0;
  clear_has_aperture();
}
inline float units::aperture() const {
  return aperture_;
}
inline void units::set_aperture(float value) {
  set_has_aperture();
  aperture_ = value;
}

// required float pca_scl = 2;
inline bool units::has_pca_scl() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void units::set_has_pca_scl() {
  _has_bits_[0] |= 0x00000002u;
}
inline void units::clear_has_pca_scl() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void units::clear_pca_scl() {
  pca_scl_ = 0;
  clear_has_pca_scl();
}
inline float units::pca_scl() const {
  return pca_scl_;
}
inline void units::set_pca_scl(float value) {
  set_has_pca_scl();
  pca_scl_ = value;
}

// repeated float pca = 3 [packed = true];
inline int units::pca_size() const {
  return pca_.size();
}
inline void units::clear_pca() {
  pca_.Clear();
}
inline float units::pca(int index) const {
  return pca_.Get(index);
}
inline void units::set_pca(int index, float value) {
  pca_.Set(index, value);
}
inline void units::add_pca(float value) {
  pca_.Add(value);
}
inline const ::google::protobuf::RepeatedField< float >&
units::pca() const {
  return pca_;
}
inline ::google::protobuf::RepeatedField< float >*
units::mutable_pca() {
  return &pca_;
}

// repeated float templates = 4 [packed = true];
inline int units::templates_size() const {
  return templates_.size();
}
inline void units::clear_templates() {
  templates_.Clear();
}
inline float units::templates(int index) const {
  return templates_.Get(index);
}
inline void units::set_templates(int index, float value) {
  templates_.Set(index, value);
}
inline void units::add_templates(float value) {
  templates_.Add(value);
}
inline const ::google::protobuf::RepeatedField< float >&
units::templates() const {
  return templates_;
}
inline ::google::protobuf::RepeatedField< float >*
units::mutable_templates() {
  return &templates_;
}

// -------------------------------------------------------------------

// channels

// required int32 id = 1;
inline bool channels::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void channels::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void channels::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void channels::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 channels::id() const {
  return id_;
}
inline void channels::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
}

// required int32 ch = 2;
inline bool channels::has_ch() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void channels::set_has_ch() {
  _has_bits_[0] |= 0x00000002u;
}
inline void channels::clear_has_ch() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void channels::clear_ch() {
  ch_ = 0;
  clear_has_ch();
}
inline ::google::protobuf::int32 channels::ch() const {
  return ch_;
}
inline void channels::set_ch(::google::protobuf::int32 value) {
  set_has_ch();
  ch_ = value;
}

// required float gain = 3;
inline bool channels::has_gain() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void channels::set_has_gain() {
  _has_bits_[0] |= 0x00000004u;
}
inline void channels::clear_has_gain() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void channels::clear_gain() {
  gain_ = 0;
  clear_has_gain();
}
inline float channels::gain() const {
  return gain_;
}
inline void channels::set_gain(float value) {
  set_has_gain();
  gain_ = value;
}

// required float agc = 4;
inline bool channels::has_agc() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void channels::set_has_agc() {
  _has_bits_[0] |= 0x00000008u;
}
inline void channels::clear_has_agc() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void channels::clear_agc() {
  agc_ = 0;
  clear_has_agc();
}
inline float channels::agc() const {
  return agc_;
}
inline void channels::set_agc(float value) {
  set_has_agc();
  agc_ = value;
}

// required float centering = 5;
inline bool channels::has_centering() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void channels::set_has_centering() {
  _has_bits_[0] |= 0x00000010u;
}
inline void channels::clear_has_centering() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void channels::clear_centering() {
  centering_ = 0;
  clear_has_centering();
}
inline float channels::centering() const {
  return centering_;
}
inline void channels::set_centering(float value) {
  set_has_centering();
  centering_ = value;
}

// required float threshold = 6;
inline bool channels::has_threshold() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void channels::set_has_threshold() {
  _has_bits_[0] |= 0x00000020u;
}
inline void channels::clear_has_threshold() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void channels::clear_threshold() {
  threshold_ = 0;
  clear_has_threshold();
}
inline float channels::threshold() const {
  return threshold_;
}
inline void channels::set_threshold(float value) {
  set_has_threshold();
  threshold_ = value;
}

// repeated .Configuration.units unit = 7;
inline int channels::unit_size() const {
  return unit_.size();
}
inline void channels::clear_unit() {
  unit_.Clear();
}
inline const ::Configuration::units& channels::unit(int index) const {
  return unit_.Get(index);
}
inline ::Configuration::units* channels::mutable_unit(int index) {
  return unit_.Mutable(index);
}
inline ::Configuration::units* channels::add_unit() {
  return unit_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::Configuration::units >&
channels::unit() const {
  return unit_;
}
inline ::google::protobuf::RepeatedPtrField< ::Configuration::units >*
channels::mutable_unit() {
  return &unit_;
}

// repeated float pca_mean = 8 [packed = true];
inline int channels::pca_mean_size() const {
  return pca_mean_.size();
}
inline void channels::clear_pca_mean() {
  pca_mean_.Clear();
}
inline float channels::pca_mean(int index) const {
  return pca_mean_.Get(index);
}
inline void channels::set_pca_mean(int index, float value) {
  pca_mean_.Set(index, value);
}
inline void channels::add_pca_mean(float value) {
  pca_mean_.Add(value);
}
inline const ::google::protobuf::RepeatedField< float >&
channels::pca_mean() const {
  return pca_mean_;
}
inline ::google::protobuf::RepeatedField< float >*
channels::mutable_pca_mean() {
  return &pca_mean_;
}

// repeated float pca_max = 9 [packed = true];
inline int channels::pca_max_size() const {
  return pca_max_.size();
}
inline void channels::clear_pca_max() {
  pca_max_.Clear();
}
inline float channels::pca_max(int index) const {
  return pca_max_.Get(index);
}
inline void channels::set_pca_max(int index, float value) {
  pca_max_.Set(index, value);
}
inline void channels::add_pca_max(float value) {
  pca_max_.Add(value);
}
inline const ::google::protobuf::RepeatedField< float >&
channels::pca_max() const {
  return pca_max_;
}
inline ::google::protobuf::RepeatedField< float >*
channels::mutable_pca_max() {
  return &pca_max_;
}

// -------------------------------------------------------------------

// radios

// required uint32 id = 1;
inline bool radios::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void radios::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void radios::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void radios::clear_id() {
  id_ = 0u;
  clear_has_id();
}
inline ::google::protobuf::uint32 radios::id() const {
  return id_;
}
inline void radios::set_id(::google::protobuf::uint32 value) {
  set_has_id();
  id_ = value;
}

// repeated .Configuration.channels channel = 2;
inline int radios::channel_size() const {
  return channel_.size();
}
inline void radios::clear_channel() {
  channel_.Clear();
}
inline const ::Configuration::channels& radios::channel(int index) const {
  return channel_.Get(index);
}
inline ::Configuration::channels* radios::mutable_channel(int index) {
  return channel_.Mutable(index);
}
inline ::Configuration::channels* radios::add_channel() {
  return channel_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::Configuration::channels >&
radios::channel() const {
  return channel_;
}
inline ::google::protobuf::RepeatedPtrField< ::Configuration::channels >*
radios::mutable_channel() {
  return &channel_;
}

// -------------------------------------------------------------------

// parameters

// required int32 signal_chain = 1;
inline bool parameters::has_signal_chain() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void parameters::set_has_signal_chain() {
  _has_bits_[0] |= 0x00000001u;
}
inline void parameters::clear_has_signal_chain() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void parameters::clear_signal_chain() {
  signal_chain_ = 0;
  clear_has_signal_chain();
}
inline ::google::protobuf::int32 parameters::signal_chain() const {
  return signal_chain_;
}
inline void parameters::set_signal_chain(::google::protobuf::int32 value) {
  set_has_signal_chain();
  signal_chain_ = value;
}

// repeated .Configuration.radios radio = 2;
inline int parameters::radio_size() const {
  return radio_.size();
}
inline void parameters::clear_radio() {
  radio_.Clear();
}
inline const ::Configuration::radios& parameters::radio(int index) const {
  return radio_.Get(index);
}
inline ::Configuration::radios* parameters::mutable_radio(int index) {
  return radio_.Mutable(index);
}
inline ::Configuration::radios* parameters::add_radio() {
  return radio_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::Configuration::radios >&
parameters::radio() const {
  return radio_;
}
inline ::google::protobuf::RepeatedPtrField< ::Configuration::radios >*
parameters::mutable_radio() {
  return &radio_;
}

// repeated int32 selected = 3;
inline int parameters::selected_size() const {
  return selected_.size();
}
inline void parameters::clear_selected() {
  selected_.Clear();
}
inline ::google::protobuf::int32 parameters::selected(int index) const {
  return selected_.Get(index);
}
inline void parameters::set_selected(int index, ::google::protobuf::int32 value) {
  selected_.Set(index, value);
}
inline void parameters::add_selected(::google::protobuf::int32 value) {
  selected_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
parameters::selected() const {
  return selected_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
parameters::mutable_selected() {
  return &selected_;
}

// -------------------------------------------------------------------

// state

// optional string default_configuration = 1;
inline bool state::has_default_configuration() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void state::set_has_default_configuration() {
  _has_bits_[0] |= 0x00000001u;
}
inline void state::clear_has_default_configuration() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void state::clear_default_configuration() {
  if (default_configuration_ != &::google::protobuf::internal::kEmptyString) {
    default_configuration_->clear();
  }
  clear_has_default_configuration();
}
inline const ::std::string& state::default_configuration() const {
  return *default_configuration_;
}
inline void state::set_default_configuration(const ::std::string& value) {
  set_has_default_configuration();
  if (default_configuration_ == &::google::protobuf::internal::kEmptyString) {
    default_configuration_ = new ::std::string;
  }
  default_configuration_->assign(value);
}
inline void state::set_default_configuration(const char* value) {
  set_has_default_configuration();
  if (default_configuration_ == &::google::protobuf::internal::kEmptyString) {
    default_configuration_ = new ::std::string;
  }
  default_configuration_->assign(value);
}
inline void state::set_default_configuration(const char* value, size_t size) {
  set_has_default_configuration();
  if (default_configuration_ == &::google::protobuf::internal::kEmptyString) {
    default_configuration_ = new ::std::string;
  }
  default_configuration_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* state::mutable_default_configuration() {
  set_has_default_configuration();
  if (default_configuration_ == &::google::protobuf::internal::kEmptyString) {
    default_configuration_ = new ::std::string;
  }
  return default_configuration_;
}
inline ::std::string* state::release_default_configuration() {
  clear_has_default_configuration();
  if (default_configuration_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = default_configuration_;
    default_configuration_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Configuration

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_parameters_2eproto__INCLUDED
