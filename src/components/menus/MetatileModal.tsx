import { Modal, ModalHeader } from "semantic-ui-react";
import "./MetatileModal.scss";
import { useState } from "react";
import Draggable from "react-draggable";

export default function MetatileModal(props: MetatileModalProps) {
  const [position, setPosition] = useState({ x: 0, y: 0 });
  const [open, setOpen] = useState(true);

  /**
   * Handle drag event
   *
   * Updates absolute x and y position of modal
   */
  const handleDrag = (_: any, ui: any) => {
    const { x, y } = ui;
    setPosition({ x, y });
  };

  return (
    <div
      className="metatile-modal-container"
      style={{
        visibility: open ? "visible" : "hidden",
      }}
    >
      <Draggable
        handle=".modal-header"
        defaultPosition={{ x: 0, y: 0 }}
        position={position}
        onDrag={handleDrag}
      >
        {/* Need container div for draggable */}
        <div className="metatile-modal">
          {/* Draggable handle */}
          <div className="modal-header">Edit Block</div>
          <div className="modal-content">
            <div className="tileset-container"></div>
            <div className="tile-container"></div>
          </div>
        </div>
      </Draggable>
    </div>
  );
}

// TODO;
// remove optional syntax
interface MetatileModalProps {
  blockId?: number;
  tileset?: string;
  palette?: string;
}
