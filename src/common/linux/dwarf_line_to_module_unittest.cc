// Copyright (c) 2009, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// dwarf_line_to_module.cc: Unit tests for google_breakpad::DwarfLineToModule.

#include "breakpad_googletest_includes.h"
#include "common/linux/dwarf_line_to_module.h"

using google_breakpad::DwarfLineToModule;
using google_breakpad::Module;
using google_breakpad::Module;

TEST(Simple, One) {
  Module m("name", "os", "architecture", "id");
  vector<Module::Line> lines;
  DwarfLineToModule h(&m, &lines);

  h.DefineFile("file1", 0x30bf0f27, 0, 0, 0);
  h.AddLine(0x6fd126fbf74f2680LL, 0x63c9a14cf556712bLL, 0x30bf0f27, 
            0x4c090cbf, 0x1cf9fe0d);

  vector<Module::File *> files;
  m.GetFiles(&files);
  EXPECT_EQ(1U, files.size());
  EXPECT_STREQ("file1", files[0]->name_.c_str());

  EXPECT_EQ(1U, lines.size());
  EXPECT_EQ(0x6fd126fbf74f2680ULL, lines[0].address_);
  EXPECT_EQ(0x63c9a14cf556712bULL, lines[0].size_);
  EXPECT_TRUE(lines[0].file_ == files[0]);
  EXPECT_EQ(0x4c090cbf, lines[0].number_);
}

TEST(Simple, Many) {
  Module m("name", "os", "architecture", "id");
  vector<Module::Line> lines;
  DwarfLineToModule h(&m, &lines);

  h.DefineDir("directory1", 0x838299ab);
  h.DefineDir("directory2", 0xf85de023);
  h.DefineFile("file1", 0x2b80377a, 0x838299ab, 0, 0);
  h.DefineFile("file1", 0x63beb4a4, 0xf85de023, 0, 0);
  h.DefineFile("file2", 0x1d161d56, 0x838299ab, 0, 0);
  h.DefineFile("file2", 0x1e7a667c, 0xf85de023, 0, 0);
  h.AddLine(0x69900c5d553b7274ULL, 0x90fded183f0d0d3cULL, 0x2b80377a,
            0x15b0f0a9U, 0x3ff5abd6U);
  h.AddLine(0x45811219a39b7101ULL, 0x25a5e6a924afc41fULL, 0x63beb4a4,
            0x4d259ce9U, 0x41c5ee32U);
  h.AddLine(0xfa90514c1dc9704bULL, 0x0063efeabc02f313ULL, 0x1d161d56,
            0x1ee9fa4fU, 0xbf70e46aU);
  h.AddLine(0x556b55fb6a647b10ULL, 0x3f3089ca2bfd80f5ULL, 0x1e7a667c,
            0x77fc280eU, 0x2c4a728cU);
  h.DefineFile("file3", -1, 0, 0, 0);
  h.AddLine(0xe2d72a37f8d9403aULL, 0x034dfab5b0d4d236ULL, 0x63beb4a5,
            0x75047044U, 0xb6a0016cU);

  vector<Module::File *> files;
  m.GetFiles(&files);
  ASSERT_EQ(5U, files.size());
  EXPECT_STREQ("directory1/file1", files[0]->name_.c_str());
  EXPECT_STREQ("directory1/file2", files[1]->name_.c_str());
  EXPECT_STREQ("directory2/file1", files[2]->name_.c_str());
  EXPECT_STREQ("directory2/file2", files[3]->name_.c_str());
  EXPECT_STREQ("file3",            files[4]->name_.c_str());

  ASSERT_EQ(5U, lines.size());

  EXPECT_EQ(0x69900c5d553b7274ULL, lines[0].address_);
  EXPECT_EQ(0x90fded183f0d0d3cULL, lines[0].size_);
  EXPECT_TRUE(lines[0].file_ == files[0]);
  EXPECT_EQ(0x15b0f0a9, lines[0].number_);

  EXPECT_EQ(0x45811219a39b7101ULL, lines[1].address_);
  EXPECT_EQ(0x25a5e6a924afc41fULL, lines[1].size_);
  EXPECT_TRUE(lines[1].file_ == files[2]);
  EXPECT_EQ(0x4d259ce9, lines[1].number_);

  EXPECT_EQ(0xfa90514c1dc9704bULL, lines[2].address_);
  EXPECT_EQ(0x0063efeabc02f313ULL, lines[2].size_);
  EXPECT_TRUE(lines[2].file_ == files[1]);
  EXPECT_EQ(0x1ee9fa4f, lines[2].number_);

  EXPECT_EQ(0x556b55fb6a647b10ULL, lines[3].address_);
  EXPECT_EQ(0x3f3089ca2bfd80f5ULL, lines[3].size_);
  EXPECT_TRUE(lines[3].file_ == files[3]);
  EXPECT_EQ(0x77fc280e, lines[3].number_);

  EXPECT_EQ(0xe2d72a37f8d9403aULL, lines[4].address_);
  EXPECT_EQ(0x034dfab5b0d4d236ULL, lines[4].size_);
  EXPECT_TRUE(lines[4].file_ == files[4]);
  EXPECT_EQ(0x75047044, lines[4].number_);
}

TEST(Filenames, Absolute) {
  Module m("name", "os", "architecture", "id");
  vector<Module::Line> lines;
  DwarfLineToModule h(&m, &lines);

  h.DefineDir("directory1", 1);
  h.DefineFile("/absolute", 1, 1, 0, 0);

  h.AddLine(1, 1, 1, 0, 0);

  vector<Module::File *> files;
  m.GetFiles(&files);
  ASSERT_EQ(1U, files.size());
  EXPECT_STREQ("/absolute", files[0]->name_.c_str());
  ASSERT_EQ(1U, lines.size());
  EXPECT_TRUE(lines[0].file_ == files[0]);
}

TEST(Filenames, Relative) {
  Module m("name", "os", "architecture", "id");
  vector<Module::Line> lines;
  DwarfLineToModule h(&m, &lines);

  h.DefineDir("directory1", 1);
  h.DefineFile("relative", 1, 1, 0, 0);

  h.AddLine(1, 1, 1, 0, 0);

  vector<Module::File *> files;
  m.GetFiles(&files);
  ASSERT_EQ(1U, files.size());
  EXPECT_STREQ("directory1/relative", files[0]->name_.c_str());
  ASSERT_EQ(1U, lines.size());
  EXPECT_TRUE(lines[0].file_ == files[0]);
}

TEST(Filenames, StrangeFile) {
  Module m("name", "os", "architecture", "id");
  vector<Module::Line> lines;
  DwarfLineToModule h(&m, &lines);

  h.DefineDir("directory1", 1);
  h.DefineFile("", 1, 1, 0, 0);
  h.AddLine(1, 1, 1, 0, 0);

  ASSERT_EQ(1U, lines.size());
  EXPECT_STREQ("directory1/", lines[0].file_->name_.c_str());
}

TEST(Filenames, StrangeDirectory) {
  Module m("name", "os", "architecture", "id");
  vector<Module::Line> lines;
  DwarfLineToModule h(&m, &lines);

  h.DefineDir("", 1);
  h.DefineFile("file1", 1, 1, 0, 0);
  h.AddLine(1, 1, 1, 0, 0);

  ASSERT_EQ(1U, lines.size());
  EXPECT_STREQ("/file1", lines[0].file_->name_.c_str());
}

TEST(Filenames, StrangeDirectoryAndFile) {
  Module m("name", "os", "architecture", "id");
  vector<Module::Line> lines;
  DwarfLineToModule h(&m, &lines);

  h.DefineDir("", 1);
  h.DefineFile("", 1, 1, 0, 0);
  h.AddLine(1, 1, 1, 0, 0);

  ASSERT_EQ(1U, lines.size());
  EXPECT_STREQ("/", lines[0].file_->name_.c_str());
}

// We should silently ignore attempts to define directory number zero,
// since that is always the compilation directory.
TEST(Errors, DirectoryZero) {
  Module m("name", "os", "architecture", "id");
  vector<Module::Line> lines;
  DwarfLineToModule h(&m, &lines);

  h.DefineDir("directory0", 0); // should be ignored
  h.DefineFile("relative", 1, 0, 0, 0);

  h.AddLine(1, 1, 1, 0, 0);

  ASSERT_EQ(1U, lines.size());
  EXPECT_STREQ("relative", lines[0].file_->name_.c_str());
}

// We should refuse to add lines with bogus file numbers. We should
// produce only one warning, however.
TEST(Errors, BadFileNumber) {
  Module m("name", "os", "architecture", "id");
  vector<Module::Line> lines;
  DwarfLineToModule h(&m, &lines);

  h.DefineFile("relative", 1, 0, 0, 0);
  h.AddLine(1, 1, 2, 0, 0); // bad file number
  h.AddLine(2, 1, 2, 0, 0); // bad file number (no duplicate warning)

  EXPECT_EQ(0U, lines.size());
}

// We should treat files with bogus directory numbers as relative to
// the compilation unit.
TEST(Errors, BadDirectoryNumber) {
  Module m("name", "os", "architecture", "id");
  vector<Module::Line> lines;
  DwarfLineToModule h(&m, &lines);

  h.DefineDir("directory1", 1);
  h.DefineFile("baddirnumber1", 1, 2, 0, 0); // bad directory number
  h.DefineFile("baddirnumber2", 2, 2, 0, 0); // bad dir number (no warning)
  h.AddLine(1, 1, 1, 0, 0);

  ASSERT_EQ(1U, lines.size());
  EXPECT_STREQ("baddirnumber1", lines[0].file_->name_.c_str());
}

// We promise not to report empty lines.
TEST(Errors, EmptyLine) {
  Module m("name", "os", "architecture", "id");
  vector<Module::Line> lines;
  DwarfLineToModule h(&m, &lines);

  h.DefineFile("filename1", 1, 0, 0, 0);
  h.AddLine(1, 0, 1, 0, 0);

  ASSERT_EQ(0U, lines.size());
}  

// We are supposed to clip lines that extend beyond the end of the
// address space.
TEST(Errors, BigLine) {
  Module m("name", "os", "architecture", "id");
  vector<Module::Line> lines;
  DwarfLineToModule h(&m, &lines);

  h.DefineFile("filename1", 1, 0, 0, 0);
  h.AddLine(0xffffffffffffffffULL, 2, 1, 0, 0);

  ASSERT_EQ(1U, lines.size());
  EXPECT_EQ(1U, lines[0].size_);
}  
