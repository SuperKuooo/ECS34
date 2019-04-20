#include "FileSystemTree.h"

struct CFileSystemTree::SImplementation{
    // You implementation here
};

struct CFileSystemTree::CEntry::SImplementation{
    // You implementation here
};

struct CFileSystemTree::CEntryIterator::SImplementation{
    // You implementation here
};

struct CFileSystemTree::CConstEntryIterator::SImplementation{
    // You implementation here
};

CFileSystemTree::CEntry::CEntry() : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::CEntry::CEntry(const CEntry &entry) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::CEntry::~CEntry(){
    // You code here
}

CFileSystemTree::CEntry &CFileSystemTree::CEntry::operator=(const CEntry &entry){
    // You code here
}

bool CFileSystemTree::CEntry::Valid() const{
    // You code here
}

std::string CFileSystemTree::CEntry::Name() const{
    // You code here
}

std::string CFileSystemTree::CEntry::FullPath() const{
    // You code here
}

std::string CFileSystemTree::CEntry::ToString() const{
    // You code here
}

CFileSystemTree::CEntry::operator std::string() const{
    // You code here
}

bool CFileSystemTree::CEntry::Rename(const std::string &name){
    // You code here
}

size_t CFileSystemTree::CEntry::ChildCount() const{
    // You code here
}

bool CFileSystemTree::CEntry::SetChild(const std::string &name, CEntryIterator &iter){
    // You code here
}

bool CFileSystemTree::CEntry::AddChild(const std::string &path, bool addall){
    // You code here
}

bool CFileSystemTree::CEntry::RemoveChild(const std::string &path){
    // You code here
}

bool CFileSystemTree::CEntry::SetData(const std::vector< char > &data){
    // You code here
}

bool CFileSystemTree::CEntry::GetData(std::vector< char > &data) const{
    // You code here
}

CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent(){
    // You code here
}

const CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent() const{
    // You code here
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::Find(const std::string &name){
    // You code here
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::Find(const std::string &name) const{
    // You code here
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::begin(){
    // You code here
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::begin() const{
    // You code here
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cbegin() const{
    // You code here
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::end(){
    // You code here
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::end() const{
    // You code here
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cend() const{
    // You code here
}

CFileSystemTree::CEntryIterator::CEntryIterator() : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::CEntryIterator::CEntryIterator(const CEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::CEntryIterator::~CEntryIterator(){
    // You code here
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator=(const CEntryIterator  &iter){
    // You code here
}

bool CFileSystemTree::CEntryIterator::operator==(const CEntryIterator &iter) const{
    // You code here
}

bool CFileSystemTree::CEntryIterator::operator!=(const CEntryIterator &iter) const{
    // You code here
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator++(){
    // You code here
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator++(int){
    // You code here
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator--(){
    // You code here
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator--(int){
    // You code here
}

CFileSystemTree::CEntry &CFileSystemTree::CEntryIterator::operator*() const{
    // You code here
}

CFileSystemTree::CEntry *CFileSystemTree::CEntryIterator::operator->() const{
    // You code here
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator() : DImplementation(std::make_unique< SImplementation >()){

}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(const CConstEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(const CEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::CConstEntryIterator::~CConstEntryIterator(){
    // You code here
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator=(const CConstEntryIterator &iter){
    // You code here
}

bool CFileSystemTree::CConstEntryIterator::operator==(const CConstEntryIterator &iter) const{
    // You code here
}

bool CFileSystemTree::CConstEntryIterator::operator!=(const CConstEntryIterator &iter) const{
    // You code here
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator++(){
    // You code here
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator++(int){
    // You code here
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator--(){
    // You code here
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator--(int){
    // You code here
}

const CFileSystemTree::CEntry &CFileSystemTree::CConstEntryIterator::operator*() const{
    // You code here
}

const CFileSystemTree::CEntry *CFileSystemTree::CConstEntryIterator::operator->() const{
    // You code here
}

CFileSystemTree::CFileSystemTree() : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::CFileSystemTree(const CFileSystemTree &tree) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
}

CFileSystemTree::~CFileSystemTree(){
    // You code here
}

CFileSystemTree &CFileSystemTree::operator=(const CFileSystemTree &tree){
    // You code here
}

CFileSystemTree::CEntry &CFileSystemTree::Root(){
    // You code here
}

const CFileSystemTree::CEntry &CFileSystemTree::Root() const{
    // You code here
}

std::string CFileSystemTree::ToString() const{
    // You code here
}

CFileSystemTree::operator std::string() const{
    // You code here
}

CFileSystemTree::CEntryIterator CFileSystemTree::Find(const std::string &path){
    // You code here
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::Find(const std::string &path) const{
    // You code here
}

CFileSystemTree::CEntryIterator CFileSystemTree::NotFound(){
    // You code here
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::NotFound() const{
    // You code here
}
