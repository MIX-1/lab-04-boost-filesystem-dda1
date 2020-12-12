// Copyright 2020 MIX-1 <danilonil1@yandex.ru>

#ifndef INCLUDE_FILE_HPP_
#define INCLUDE_FILE_HPP_

#include <boost/filesystem.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <utility>

using path = boost::filesystem::path;
using dir_iterator = boost::filesystem::directory_iterator;
using fs_error = boost::filesystem::filesystem_error;
using string = std::string;
using std::cout;

struct Account {
 public:
  explicit Account(const path&);

  explicit Account(
      string& in_account_name,
      int data_ = 0,
      string broker_name_ = "",
      bool correct_type_ = true);

  static string cutter(const string&);

  static inline bool is_correct_file(const path&);

  void setter(const string&, const string&, const string&);

  int get_data();

  string get_account_name();

  string get_broker_name();

  void set_data(int);

  void set_broker_name(string);

  bool get_correct_type();

  void print(std::ostream&);
 private:
  int data;
  string account_name;
  string broker_name;
  bool correct_type;
};

class File {
 public:
  explicit File(const path&);

  std::vector<string> sort_account_name();

  void set_accounts_unique(std::vector<string>);

  void print_accounts_all(std::ostream&);

  void print_accounts_unique(std::ostream&);

  void exit_test(const path& dir_path);

  friend std::ostream& operator<<(std::ostream&, File&);
 private:
  std::vector<Account> accounts_all;
  std::vector<Account> accounts_unique;
  std::vector<int> count_accounts;
};
#endif // INCLUDE_FILE_HPP_
