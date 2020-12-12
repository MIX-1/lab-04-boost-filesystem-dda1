// Copyright 2020 MIX-1 <danilonil1@yandex.ru>

#include <File.hpp>

File::File(const path& path_) {
  for (auto&& item : dir_iterator(path_))
    exit_test(item);
  set_accounts_unique(sort_account_name());
}
void File::exit_test(const path& dir_path) {
  try {
    if (exists(dir_path)) {
      if (is_regular_file(dir_path)) {
        accounts_all.emplace_back(dir_path);
      } else if (is_directory(dir_path)) {
        for (auto&& item : dir_iterator(dir_path)) {
          exit_test(item);
        }
      } else if (is_symlink(dir_path)) {
        exit_test(read_symlink(dir_path));
      } else {
        throw std::runtime_error("it is unidentified exists");
      }
    } else {
      throw std::runtime_error("bad exist for this path");
    }
  } catch (const fs_error& e) {
    cout << e.what() << "\n";
  }
}

std::vector<string> File::sort_account_name() {
  std::vector<string> accounts_name;
  for(auto & account : accounts_all){
    if(!account.get_account_name().empty())
      accounts_name.push_back(account.get_account_name());
  }
  std::sort(accounts_name.begin(), accounts_name.end());
  return accounts_name;
}

void File::set_accounts_unique(std::vector<string> accounts_name) {
  accounts_unique.emplace_back(accounts_name[0]);
  count_accounts.push_back(1);
  for(size_t i = 1; i < accounts_name.size(); ++i){
    if (accounts_name[i] != accounts_name[i-1]){
      accounts_unique.emplace_back(accounts_name[i]);
      count_accounts.push_back(1);
    } else {
      ++count_accounts.back();
    }
  }
  int temp_data = 0;
  string temp_broker;
  for(auto & account_unique : accounts_unique){
    for(auto & account : accounts_all){
      if(account.get_account_name() == account_unique.get_account_name()){
        if(account.get_data() > temp_data) {
          temp_broker = account.get_broker_name();
          temp_data = account.get_data();
        }
      }
    }
    account_unique.set_data(temp_data);
    temp_data = 0;
    account_unique.set_broker_name(temp_broker);
    temp_broker = "";
  }
}
void File::print_accounts_all(std::ostream& tab) {
  tab << "------------------ All financial files: ------------------------\n\n";
  for(auto & account : accounts_all) {
    if (account.get_correct_type()) account.print(tab);
  }
}
void File::print_accounts_unique(std::ostream& tab) {
  tab << "\n\n----------------- Users accounts info: ---------------------\n\n";
  for (size_t i = 0; i < accounts_unique.size(); ++i){
    tab << "broker :" << accounts_unique[i].get_broker_name() << " account :" <<
        accounts_unique[i].get_account_name() << " files: " <<
        count_accounts[i] << " last date:" << accounts_unique[i].get_data() <<
        "\n";
  }
}
std::ostream& operator<<(std::ostream& tab, File& file) {
  file.print_accounts_all(tab);
  file.print_accounts_unique(tab);
  return tab;
}


Account::Account(const path& file) {
  if (is_correct_file(file)) {
    correct_type = true;
    string stem_str = file.stem().string();
    setter(cutter(stem_str),
           cutter(
               stem_str.substr(
                   0,
                   stem_str.size() - cutter(stem_str).size())),
           file.parent_path().filename().string());
    } else {
      correct_type = false;
    }
}
bool Account::is_correct_file(const path& in) {
  string in_str = in.stem().string();
  size_t count_ = 0;
  bool mask_type = false;
  for (const auto& ch : in_str) {
    if (ch == '_') {
      ++count_;
    }
    switch (count_) {
      case 0:
        if (ch < 65 || ch > 90 || ch < 97 || ch > 122)
          mask_type = false;
        else
          mask_type = true;
        break;
      case 1: case 2:
        if (ch < 49 || ch > 57)
          mask_type = false;
        else
          mask_type = true;
        break;
      default:
        mask_type = false;
    }
  }
  if (count_ != 2) {
    mask_type = false;
  }
  if (in.extension() == ".txt"
      && in.stem().extension() != ".old"
      && mask_type) {
    return true;
  } else {
    return false;
  }
}

string Account::cutter(const string& in) {
  //balance_XXXXXXXX_YYYYMMDD
  string out;
  for (int i = in.length(); i > 0; --i){
    if (in[i] == '_')
      break;
    out += in[i];
  }
  int len = out.length();
  int n = len-1;
  for (int i=0; i<(len/2); ++i){
    std::swap(out[i],out[n]);
    --n;
  }
  return out;
}
void Account::setter(const string& in_data,
                     const string& in_account_name,
                     const string& in_broker_name) {
  data = atoi(in_data.c_str());
  account_name = in_account_name;
  broker_name = in_broker_name;
}
void Account::print(std::ostream& tab) {
  tab << broker_name << "\t" << "balance_" << account_name << "_" << data <<
      ".txt\n";
}
int Account::get_data() { return data; }
string Account::get_account_name() { return account_name; }
string Account::get_broker_name() { return broker_name; }
void Account::set_data(int in_data) { data = in_data;}
void Account::set_broker_name(string in_broker_name) {
  broker_name = std::move(in_broker_name);
}
Account::Account(string& in_account_name, int data_,
                 string broker_name_, bool correct_type_)
    : data(data_), account_name(in_account_name),
      broker_name(std::move(broker_name_)), correct_type(correct_type_) {}
bool Account::get_correct_type() { return correct_type; }
