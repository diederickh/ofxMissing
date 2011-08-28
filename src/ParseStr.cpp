#include "ParseStr.h"

ParseStrResult parse_str(string str) {
	ParseStrResult result;
	StringTokenizer raw_vars(str, "&", StringTokenizer::TOK_TRIM);
	for(int i = 0; i < raw_vars.count(); ++i) {
		try {
			StringTokenizer split_var(raw_vars[i], "=");
			if(split_var.count() != 2) {
				cout << "parse_str error: split_var not var=value pair" << endl;
				continue;
			}
			//cout << split_var[0] << " = " << split_var[1] << endl;
			result.vars.insert(std::pair<string,string>(split_var[0], split_var[1]));
		}
		catch(...) {
			cout << "parse_str: unhandled exception" << endl;
		}
	}
	return result;
}


