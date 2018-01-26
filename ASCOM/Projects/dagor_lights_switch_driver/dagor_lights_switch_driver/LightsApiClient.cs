using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ASCOM.DagorApiClient;


namespace ASCOM.DagorLights
{

    public class ReprState : IRepr
    {
        public int n { get; set; }
    }

    class LightsApiClient : ApiClient<DefaultReadyRepr>
    {
        public LightsApiClient(string Proto, string Server, int Port) : base(Proto, Server, Port)
        { }

        protected override string _DevPath()
        {
            return "lights";
        }

        public bool IsIdle
        {
            get
            {
                return true;
            }
        }
        
        public int N
        {
            get
            {
                try
                {
                    ReprState repr_state = ExecuteGET<ReprState>("state");
                    return repr_state.n;
                }
                catch (Exception)
                {
                    // TODO log excaption
                    throw new NotConnectedException();
                }
            }
            set
            {
                try
                {
                    ReprState repr_state = new ReprState();
                    repr_state.n = value;

                    ReprState repr_n_response = ExecutePUT<ReprState, ReprState>("state", repr_state);
                }
                catch (Exception)
                {
                    // TODO log exception
                    throw new NotConnectedException();
                }
            }
        } 

        public bool getSwitch(int n)
        {
            try
            {
                bool repr_state = ExecuteGET<bool>(n.ToString());
                return repr_state;
            }
            catch (Exception)
            {
                // TODO log exception
                throw new NotConnectedException();
            }
        }

        public bool setSwitch(int n, bool state)
        {
            try
            {
                bool repr_state = ExecutePUT<bool, bool>(n.ToString(), state);
                return repr_state;
            }
            catch (Exception)
            {
                // TODO log exception
                throw new NotConnectedException();
            }
        }
    }
}
