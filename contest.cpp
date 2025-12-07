#include <bits/stdc++.h>
using namespace std;

#define tawfiq() ios_base::sync_with_stdio(false); cin.tie(nullptr);
using ll = long long;

const int MX = 2000000 + 123;   // needs 2*n + extra
const ll MOD1 = 1000000007LL;
const ll MOD2 = 1000000009LL;
const ll BASE = 137;

ll pw1[MX], pw2[MX], inv1[MX], inv2[MX];
ll fh1[MX], fh2[MX], rh1[MX], rh2[MX];

ll mul(ll a, ll b, ll mod){ return (a % mod) * (b % mod) % mod; }

ll addmod(ll a, ll b, ll mod){
    a %= mod; b %= mod;
    ll r = a + b;
    if(r >= mod) r -= mod;
    if(r < 0) r += mod;
    return r;
}

ll binexp(ll a, ll b, ll mod){
    ll r = 1;
    while(b){
        if(b & 1) r = mul(r, a, mod);
        a = mul(a, a, mod);
        b >>= 1;
    }
    return r;
}

void precalc(){
    pw1[0] = pw2[0] = 1;
    for(int i = 1; i < MX; i++){
        pw1[i] = mul(pw1[i-1], BASE, MOD1);
        pw2[i] = mul(pw2[i-1], BASE, MOD2);
    }

    ll ib1 = binexp(BASE, MOD1 - 2, MOD1);
    ll ib2 = binexp(BASE, MOD2 - 2, MOD2);

    inv1[0] = inv2[0] = 1;
    for(int i = 1; i < MX; i++){
        inv1[i] = mul(inv1[i-1], ib1, MOD1);
        inv2[i] = mul(inv2[i-1], ib2, MOD2);
    }
}

void build_fhash(const string &s){
    int n = s.size();
    for(int i = 0; i < n; i++){
        ll v = s[i] + 1;
        fh1[i] = addmod((i ? fh1[i-1] : 0), mul(v, pw1[i], MOD1), MOD1);
        fh2[i] = addmod((i ? fh2[i-1] : 0), mul(v, pw2[i], MOD2), MOD2);
    }
}

void build_rhash(const string &s){
    int n = s.size();
    for(int i = 0; i < n; i++){
        ll v = s[i] + 1;
        rh1[i] = addmod((i ? rh1[i-1] : 0), mul(v, pw1[i], MOD1), MOD1);
        rh2[i] = addmod((i ? rh2[i-1] : 0), mul(v, pw2[i], MOD2), MOD2);
    }
}

pair<ll,ll> get_hash(ll h1[], ll h2[], int l, int r){
    ll x1 = h1[r];
    if(l) x1 = addmod(x1, -h1[l-1], MOD1);
    x1 = mul(x1, inv1[l], MOD1);

    ll x2 = h2[r];
    if(l) x2 = addmod(x2, -h2[l-1], MOD2);
    x2 = mul(x2, inv2[l], MOD2);

    return {x1, x2};
}

string s, rs;
int n;

bool exists_len(int L, int &pos){
    for(int i = 0; i + L - 1 < n; i++){
        int j = i + L - 1;

        auto fwd = get_hash(fh1, fh2, i, j);

        int rl = n - 1 - j;
        int rr = n - 1 - i;

        auto rev = get_hash(rh1, rh2, rl, rr);

        if(fwd == rev){
            pos = i;
            return true;
        }
    }
    return false;
}

int main(){
    tawfiq();
    precalc();

    string s1;
    cin >> s1;

    // Transform with separators: "#a#b#b#a#"
    s.reserve(2 * s1.size() + 5);
    for(char c : s1){
        s.push_back('#');
        s.push_back(c);
    }
    s.push_back('#');

    n = s.size();

    // Build forward + reverse hash
    build_fhash(s);
    rs = s;
    reverse(rs.begin(), rs.end());
    build_rhash(rs);

    int lo = 1, hi = n, bestLen = 1, bestPos = 0;

    while(lo <= hi){
        int mid = (lo + hi) / 2;
        if(mid % 2 == 0) mid--; // only odd lengths possible now (centered)

        int pos;
        if(mid > 0 && exists_len(mid, pos)){
            if(mid > bestLen){
                bestLen = mid;
                bestPos = pos;
            }
            lo = mid + 2; // jump by 2 (odd lengths only)
        }else{
            hi = mid - 2;
        }
    }

    // Extract palindrome from transformed string
    string result;
    for(int i = bestPos; i < bestPos + bestLen; i++){
        if(s[i] != '#') result.push_back(s[i]);
    }

    cout << result << "\n";
}
#include <bits/stdc++.h>
using namespace std;
using u128 = unsigned __int128;
using u64 = unsigned long long;

u64 mulmod(u64 a, u64 b, u64 mod) {
    return (u128)a * b % mod;
}

u64 power(u64 a, u64 d, u64 mod) {
    u64 r = 1;
    while(d) {
        if(d & 1) r = mulmod(r, a, mod);
        a = mulmod(a, a, mod);
        d >>= 1;
    }
    return r;
}

bool miller_rabin(u64 n) {
    if(n < 2) return false;
    for(u64 p : {2,3,5,7,11,13,17,19,23,29,31,37})
        if(n % p == 0) return n == p;

    u64 d = n - 1, s = 0;
    while((d & 1) == 0) d >>= 1, s++;

    for(u64 a : {2,325,9375,28178,450775,9780504,1795265022}) {
        u64 x = power(a % n, d, n);
        if(x == 1 || x == n-1) continue;

        bool composite = true;
        for(int i = 1; i < s; i++) {
            x = mulmod(x, x, n);
            if(x == n-1) {
                composite = false;
                break;
            }
        }
        if(composite) return false;
    }
    return true;
}

int main() {
    long long n;
    cin >> n;
    if(miller_rabin(n)) cout << "Prime\n";
    else cout << "Composite\n";
}
 ll l,r;
   l=100000000,r=100000100;

  // sieve(h);
   int sz=r-l+1;
   cout<<sz<<endl;
   vi v(sz,1);

   for(auto u:primes)
   {
   if(1ll*u*u<=r)
   // cout<<u<<endl;
     {ll lp=(l/u)*u;
 if(lp<l)  lp+=u;
      if(lp-l<sz) v[lp-l]=0ll;
    for(;lp<=r;lp+=u)
       {
     //  cout<<lp<<" "<<endl;
          if(lp!=u) v[lp-l]=0;

       }

     }
   }
   for(int i=0;i<sz;i++)
   {
       if(v[i])cout<<i+l<<" ";

