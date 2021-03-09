int main() {
  output "1 comando";
  output "2 comando";
  {
    output "3 comando";
    output "4 comando";
    output "5 comando";
    output "6 comando";
    output "7 comando";
    {
      output "XX comando";
      output "ZZ comando";
      output "YY comando";
    };
    output "AA comando";
    output "BB comando";
  };
  output "8 comando";
  output "9 comando";
}