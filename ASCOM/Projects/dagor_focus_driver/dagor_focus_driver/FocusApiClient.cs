using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ASCOM.DagorApiClient;


namespace ASCOM.DagorFocus
{


    public class ReprState : IRepr
    {
        public bool can_go_up { get; set; }
        public bool stopping_hard { get; set; }
        public bool idle { get; set; }
        public bool intent_up { get; set; }
        public bool can_go_dn { get; set; }
        public bool moving_up { get; set; }
        public int position { get; set; }
        public bool moving_dn { get; set; }
        public bool intent_dn { get; set; }
    }


    class FocusApiClient : ApiClient<DefaultReadyRepr>
    {
        public FocusApiClient(string Proto, string Server, int Port) : base(Proto, Server, Port)
        { }

        protected override string _DevPath() {
            return "focus"; }

        public bool IsIdle
        {
            get
            {
                return true;
            }
        }



    }
}
