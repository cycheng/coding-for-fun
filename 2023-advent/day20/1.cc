// https://adventofcode.com/2023/day/20

#include <bits/stdc++.h>
using namespace std;

/* format

broadcaster -> a, b, c
%a -> b
%b -> c
%c -> inv
&inv -> a

*/

// To get the cables warmed up, the Elves have pushed the button 1000 times. How
// many pulses got sent as a result (including the pulses sent by the button
// itself)?
enum {
  FlipFlop = 1,
  Conj,
  Broadcast,
};

string getTypeStr(int type) {
  switch (type) {
  case FlipFlop: return "FlipFlop";
  case Conj: return "Conj";
  case Broadcast: return "Broadcast";
  default: return "untyped";
  }
}

struct Module {
  vector<shared_ptr<Module>> dest;
  vector<shared_ptr<Module>> src;
  // virtual void update(bool highPulse) = 0;
  uint8_t type = 0;
  bool highPulse = false;
  unordered_map<Module *, bool> inputs; // conj only.
  string name;
  void print(bool newPulse) {
    cout << name << " " << getTypeStr(type) << "\n";
    string pulseStr = newPulse ? "high" : "low";
    for (auto &iter : dest) {
      cout << "  " << pulseStr << " -> '" << iter->name << "' "
           << getTypeStr(iter->type) << "\n";
    }
  }
};

#if 0
struct FlipFlop : public Module {
  void update(bool highPulse) override {}
};

struct Conj : public Module {
  void update(bool highPulse) override {}
};

struct Broadcast : public Module {
  void update(bool highPulse) override {}
};
#endif

// <propagate pulse, pulse value>
pair<bool, bool> sendPulse(Module &mod, Module *srcMod, const bool highPulse,
                           int loopCnt) {
  const uint8_t type = mod.type;

  // Flip-flop modules (prefix %) are either on or off; they are initially off.
  // - receives a high pulse, it is ignored
  // - receives a low pulse, it flips between on and off.
  //
  // - If it was off, it turns on and sends a high pulse
  // - If it was on, it turns off and sends a low pulse.
  if (type == FlipFlop) {
    if (!highPulse) {
      mod.highPulse = !mod.highPulse;
      return {true, mod.highPulse};
    }
    return {false, false};
  }

  // Conjunction modules (prefix &) remember the type of the most recent pulse
  // received from each of their connected input modules;
  //
  // they initially default to remembering a low pulse for each input.
  //
  // When a pulse is received, the conjunction module first updates its memory
  // for that input.
  //
  // if it remembers high pulses for all inputs, it sends a low pulse;
  // otherwise, it sends a high pulse.
  if (type == Conj) {
    mod.inputs[srcMod] = highPulse;

    for (auto iter : mod.inputs)
      if (!iter.second)
        return {true, true};

    // for (auto iter : mod.src)
    //   if (!iter->highPulse)
    //     return {true, true};
    return {true, false};
  }

  // broadcast module
  // When it receives a pulse, it sends the same pulse to all of its destination
  // modules.
  if (type == Broadcast) {
    mod.highPulse = highPulse;
    return {true, highPulse};
  }

  return {false, false};
}

// using GraphMap = unordered_map<string, shared_ptr<Module>>;
using GraphMap = map<string, shared_ptr<Module>>;
using VisitedSet = unordered_set<string>;
using ScoreMap = unordered_map<Module *, int>;

struct Send {
  bool pulse = false;
  Module *src = nullptr;
  Module *dst = nullptr;

  Send(bool p, Module *s, Module *d) : pulse(p), src(s), dst(d) {}
  Send() = default;
};

// button module. When you push the button, a single low pulse is sent directly
// to the broadcaster module.
// you must wait until all pulses have been delivered and fully handled before
// pushing it again.
pair<uint32_t, uint32_t> pushButton(const GraphMap &graph, bool startingPulse,
                                    int loopCount, ScoreMap &scores,
                                    Module *targetM) {
  uint32_t numHigh = 0, numLow = 1;
  auto iter = graph.find("broadcaster");
  queue<Send> modqueue;
  modqueue.emplace(startingPulse, nullptr, iter->second.get());
  while (!modqueue.empty()) {
    int len = (int)modqueue.size();
    for (int i = 0; i < len; ++i) {
      Module *srcMod = modqueue.front().src;
      Module *mod = modqueue.front().dst;
      bool highPulse = modqueue.front().pulse;

      modqueue.pop();
      pair<bool, bool> result = sendPulse(*mod, srcMod, highPulse, loopCount);

      if (mod == targetM) {
        if (highPulse && scores.find(srcMod) == scores.end())
          scores[srcMod] = loopCount;
      }
      // if (result.first)
      //   mod->print(result.second);

      if (result.first) {
        if (result.second)
          numHigh += mod->dest.size();
        else
          numLow += mod->dest.size();

        for (auto &destIter : mod->dest) {
          modqueue.emplace(result.second, mod, destIter.get());
        }
      }
    }
    // cout << "\n";
  }
  return {numHigh, numLow};
}

// ref:
// https://www.reddit.com/r/adventofcode/comments/18mmfxb/2023_day_20_solutions/?sort=confidence
//
// david3x3x3's soluction (python) is very elegant...
//
// morgoth1145:
//   "the 4 output modules from broadcaster, and the 4 input conjunction modules
//    to a conjunction module which then was the sole input to rx. Assuming that
//    indicated 4 distinct subgraphs with their own periods (I did spend a
//    little bit of time checking one of the branches to make sure it was
//    plausible) I just had to detect those periods and take the LCM to get the
//    answer."
//
// CY:
// 1. only one single conjunction connects to 'rx', in my test, it is 'bb'
// 2. 4 independent conjunctions (from 4 subgraphs) connects to 'bb'
// 3. check num cycles are required for each bb's input send a high pulse.
// 4. lcm the 4 cycles we found, in my test, they are:
//    [3907, 3823, 3733, 3797]
//    => 211712400442661
uint64_t solve(const GraphMap &graph) {
  VisitedSet visited;
  vector<uint64_t> numPulsesList;
  uint64_t high = 0, low = 0;
  ScoreMap scores;
  Module *targetM = graph.find("rx")->second->src.front().get();
  assert(graph.find("rx")->second->src.size() == 1);

  int i = 1;
  while (1) {
    auto numPulses = pushButton(graph, false /*low pulse*/, i, scores, targetM);
    high += numPulses.first;
    low += numPulses.second;
    if (scores.size() == targetM->src.size())
      break;
    ++i;
  }
  uint64_t ans = 1;
  for (const auto &iter : scores) {
    ans *= iter.second;
  }
  cout << "ans = " << ans << "\n";
  return ans;
}

// Pulses are always processed in the order they are sent.
void parse(GraphMap &graph, string str) {
  size_t begin = 0;
  size_t end = str.find(' ');
  string name = str.substr(0, end - begin);

  shared_ptr<Module> mod;
  uint8_t type = 0;

  if (name == "broadcaster") {
    type = Broadcast;
  } else {
    if (name[0] == '%')
      type = FlipFlop;
    else
      type = Conj;
    name.erase(name.begin());
  }

  auto iter = graph.find(name);
  if (iter == graph.end()) {
    mod = make_shared<Module>();
    graph[name] = mod;
    mod->name = name;
  } else {
    mod = iter->second;
  }

  mod->type = type;

  // broadcaster -> a, b, c
  begin = str.find(' ', end + 1) + 1;
  while (end != string::npos) {
    end = str.find(',', end + 1);
    string dest;
    if (end != string::npos) {
      dest = str.substr(begin, end - begin);
      begin = end + 2;
    } else {
      dest = str.substr(begin);
    }

    shared_ptr<Module> destM;
    auto iter = graph.find(dest);
    if (iter == graph.end()) {
      destM = make_shared<Module>();
      graph[dest] = destM;
      destM->name = dest;
    } else {
      destM = iter->second;
    }
    mod->dest.push_back(destM);

    // debug only.
    {
      for (auto iter : destM->src)
        assert(iter.get() != destM.get());
    }

    destM->src.push_back(mod);
  }
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  GraphMap graph;
  for (std::string line; getline(input, line);) {
    parse(graph, line);
  }
  graph["rx"]->type = Conj;
  for (auto &iter : graph) {
    if (iter.second->type != Conj)
      continue;

    for (auto &src : iter.second->src) {
      iter.second->inputs[src.get()] = false;
    }
  }

  solve(graph);
  return 0;
}
